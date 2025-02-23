#include "Characters/CombatCharacter.h"
#include "Characters/SpriteCharacter.h"
#include "Characters/Ally.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMechanics/Effect.h"
#include "GameMechanics/Skill.h"
#include "GameMechanics/AttributeComponent.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "Components/SceneComponent.h"
#include "PaperFlipbookComponent.h"
#include "UI/CharacterWidgetComponent.h"
#include "UI/CharacterWidget.h"
#include "UI/DamageIndicatorWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ACombatCharacter::ACombatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CharacterWidgetComponent = CreateDefaultSubobject<UCharacterWidgetComponent>(TEXT("Characer Widget"));
	CharacterWidgetComponent->SetupAttachment(GetRootComponent());
	DamageIndicatorComponent = CreateDefaultSubobject<UDamageIndicatorWidgetComponent>(TEXT("Damage Indicator"));
	DamageIndicatorComponent->SetupAttachment(GetRootComponent());

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	ParticleSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("Particle Spawner"));
	ParticleSpawner->SetupAttachment(GetRootComponent());
}

void ACombatCharacter::Tick(float DeltaTime)
{
	if(!bWhirling)
	{
		Super::Tick(DeltaTime);
	}
	if (bWhirling && GetSprite())
	{
		FRotator Rotation = FRotator(0.f, 180.f, 0.f);
		GetSprite()->AddLocalRotation(Rotation * DeltaTime * 10);
	}
	if (CurrentGameMode && CurrentGameMode->CurrentMode==ECurrentMode::ECM_Combat && bShouldMoveToTarget && CombatTarget)
	{
		if (InTargetRange(CombatTarget, 100.f) && GetCharacterMovement()->Velocity.Size2D() == 0)
		{
			CombatTarget = nullptr;
			bShouldMoveToTarget = false;
			OnTargetReached.Broadcast(this);
		}
		else
		{
			MoveToTarget(CombatTarget);
		}
	}
	if ((CurrentGameMode && CurrentGameMode->CurrentMode == ECurrentMode::ECM_Combat && bShouldMoveToLocation))
	{
		if (InPosition(TargetLocation, 80.f) && GetCharacterMovement()->Velocity.Size2D() == 0)
		{
			bShouldMoveToLocation = false;
			OnLocationReached.Broadcast(this);
		}
		else
		{
			MoveToLocation(TargetLocation);
		}
	}
}

void ACombatCharacter::SetExperience(int32 NewExperience)
{
	Experience = NewExperience;
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();
	HideWidget();
	if (CharacterWidgetComponent) CharacterWidgetComponent->HideRageBar();
	UpdateHealthBar();
	UpdateManaBar();
}

void ACombatCharacter::ShowWidget()
{
	if(CharacterWidgetComponent)
	{
		CharacterWidgetComponent->SetVisibility(true);
	}
}

void ACombatCharacter::HideWidget()
{
	if(CharacterWidgetComponent)
	{
		CharacterWidgetComponent->SetVisibility(false);
	}
}

void ACombatCharacter::ShowArrow()
{
	if (CharacterWidgetComponent)
	{
		CharacterWidgetComponent->ShowArrow();
	}
}

void ACombatCharacter::SetArrowToWhite()
{
	if (CharacterWidgetComponent)
	{
		CharacterWidgetComponent->SetArrowToWhite();
	}
}

void ACombatCharacter::SetArrowToColor()
{
}

void ACombatCharacter::HideArrow()
{
	if(CharacterWidgetComponent)
	{
		CharacterWidgetComponent->HideArrow();
	}
}

void ACombatCharacter::UpdateEffectIcons()
{
	if (CharacterWidgetComponent)
	{
		CharacterWidgetComponent->UpdateEffectIcons(Effects);
	}
}

void ACombatCharacter::StartTurn()
{
	UE_LOG(LogTemp, Log, TEXT("[ACombatCharacter] Start turn for character: %s"), *GetName());
	Turn++;
	ApplyEffectsPreTurn();
}

void ACombatCharacter::EndTurn()
{
	UE_LOG(LogTemp, Log, TEXT("[ACombatCharacter] End turn for character: %s"), *GetName());
	ApplyEffectsPostTurn();
	UpdateEffectIcons();
	OnCharacterEndTurn.Broadcast(this);
}

void ACombatCharacter::ApplyEffectsPreTurn()
{
	for (UEffect* Effect : Effects)
	{
		FString AffectedAttribute = Effect->GetAffectedAttribute();
		/*if (Effect->TurnMode == EEffectTurnMode::EETM_BuffDebuff && !AffectedAttribute.IsEmpty())
		{
			uint8 NewStatValue = *Attributes->Stats.Find(AffectedAttribute) + Effect->GetValue();
			Attributes->Stats.Emplace(AffectedAttribute, NewStatValue);
			Effect->bApplied = true;
		}
		if (Effect->TurnMode == EEffectTurnMode::EETM_Passive)
		{
			Effect->bApplied = true;
		}*/
		Effect->bApplied = true;
	}
	UpdateEffectIcons();
}

void ACombatCharacter::ApplyEffectsPostTurn()
{
	if (ActorHasTag(FName("Dead")))
	{
		Effects.Empty();
		UpdateEffectIcons();
		return;
	}
	for (UEffect* Effect : Effects)
	{
		if (Effect->TurnMode == EEffectTurnMode::EETM_EndOfTurn && Effect->GetDuration()>0)
		{
			float EffectValue = Effect->GetValue();
			if (Effect->bHeal)
			{
				ReceiveHealing(EffectValue);
			}
			else
			{
				GetHitEffect(Effect);
			}
			Effect->SetDuration(Effect->GetDuration() - 1);
			UpdateHealthBar();
		}
		FString AffectedAttribute = Effect->GetAffectedAttribute();
		if (Effect->TurnMode == EEffectTurnMode::EETM_BuffDebuff && !AffectedAttribute.IsEmpty() && Effect->bApplied)
		{
			//uint8 NewStatValue = *Attributes->Stats.Find(AffectedAttribute) - Effect->GetValue();
			//Attributes->Stats.Emplace(AffectedAttribute, NewStatValue);
			Effect->SetDuration(Effect->GetDuration() - 1);
			if (Effect->GetDuration() == 0)
			{
				StatModifiers.Remove(AffectedAttribute);
			}
		}
		if(Effect->TurnMode==EEffectTurnMode::EETM_Passive && Effect->bApplied)
		{
			Effect->SetDuration(Effect->GetDuration() - 1);
		}
		if (Effect->GetDuration() == 0)
		{
			Effects.Remove(Effect);
			if (Effect->EffectType == EEffectType::EET_Defender)
			{
				Tags.Remove(FName("Defender"));
			}
			if (IsA(AAlly::StaticClass()) && Effect->EffectType==EEffectType::EET_Drunk)
			{
				AAlly* CharacterAsAlly = Cast<AAlly>(this);
				if (CharacterAsAlly)
				{
					CharacterAsAlly->RoundsDrank = 0;
				}
			}
		}
	}
	UpdateEffectIcons();
	UpdateRageBar();
}

void ACombatCharacter::UpdateHealthBar()
{
	if(CharacterWidgetComponent)
	{
		CharacterWidgetComponent->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

void ACombatCharacter::UpdateManaBar()
{
	if (CharacterWidgetComponent)
	{
		CharacterWidgetComponent->SetManaPercent(Attributes->GetManaPercent());
	}
}

void ACombatCharacter::UpdateRageBar()
{
	if (CharacterWidgetComponent)
	{
		CharacterWidgetComponent->SetRagePercent(Attributes->GetRagePercent());
	}
}

void ACombatCharacter::ShowSpecialInfoText(FString SpecialText)
{
	if (CharacterWidgetComponent)
	{
		if (SpecialText.Len() > 13)
		{
			SpecialText = SpecialText.Left(8) + "(...)!";
		}
		CharacterWidgetComponent->SetSpecialInfoText(SpecialText);
		CharacterWidgetComponent->ShowSpecialInfoText();
		GetWorldTimerManager().SetTimer(SpecialTextTimer, CharacterWidgetComponent, &UCharacterWidgetComponent::HideSpecialInfoText, 1.5f);
	}
}

void ACombatCharacter::GetHit(float Damage, EDamageType DamageType)
{
	if (GetEffectOfType(EEffectType::EET_Deathwish))
	{
		Damage *= 2;
	}
	Attributes->ReceiveDamage(Damage);
	if (DamageIndicatorComponent)
	{
		DamageIndicatorComponent->DisplayDamage(Damage);
	}

	//sounds
	if (HitSound && DamageType == EDamageType::EDT_Normal)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
	if (HitSoundDarkMagic && DamageType == EDamageType::EDT_DarkMagic)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSoundDarkMagic, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
	if (HitSoundBrightMagic && DamageType == EDamageType::EDT_BrightMagic)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSoundBrightMagic, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
	if (HitSoundBonk && DamageType == EDamageType::EDT_Bonk)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSoundBonk, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
	if (HitSoundSlap && DamageType == EDamageType::EDT_Slap)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSoundSlap, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
	if (HitSoundChem && DamageType == EDamageType::EDT_Chemical)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSoundChem, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
	if (HitSoundSplash && DamageType == EDamageType::EDT_Splash)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSoundSplash, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}

	//vfx
	if (HitParticles && DamageType == EDamageType::EDT_Normal)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ParticleSpawner->GetComponentLocation(), ParticleSpawner->GetComponentRotation());
	}
	if (HitParticlesDarkMagic && DamageType == EDamageType::EDT_DarkMagic)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticlesDarkMagic, ParticleSpawner->GetComponentLocation(), ParticleSpawner->GetComponentRotation());
	}
	if (HitParticlesBrightMagic && DamageType == EDamageType::EDT_BrightMagic)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticlesBrightMagic, ParticleSpawner->GetComponentLocation(), ParticleSpawner->GetComponentRotation());
	}
	if (HitParticlesBang && (DamageType == EDamageType::EDT_Bonk || DamageType == EDamageType::EDT_Slap || DamageType == EDamageType::EDT_AOE))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticlesBang, ParticleSpawner->GetComponentLocation(), ParticleSpawner->GetComponentRotation());
	}
	if (HitParticlesChem && DamageType == EDamageType::EDT_Chemical)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticlesChem, ParticleSpawner->GetComponentLocation(), ParticleSpawner->GetComponentRotation());
	}
	if (HitParticlesSplash && DamageType == EDamageType::EDT_Splash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticlesSplash, ParticleSpawner->GetComponentLocation(), ParticleSpawner->GetComponentRotation());
	}

	UpdateHealthBar();

	if (Attributes->GetHealth() == 0)
	{
		Die();
	}
}

void ACombatCharacter::GetHitEffect(UEffect* Effect)
{
	SpawnEffectParticles(Effect);
	Attributes->ReceiveDamage(Effect->GetValue());

	if (DamageIndicatorComponent)
	{
		DamageIndicatorComponent->DisplayDamage(Effect->GetValue());
	}

	//efekty
	if (Effect->EffectSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Effect->EffectSound, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}

	UpdateHealthBar();

	if (ActorHasTag(FName("MainCharacter")))
	{
		GetMad();
	}

	if (Attributes->GetHealth() == 0)
	{
		Die();
	}
}

void ACombatCharacter::GetHitVortex(float Damage)
{
	Attributes->ReceiveDamage(Damage);
	if (DamageIndicatorComponent)
	{
		DamageIndicatorComponent->DisplayDamage(Damage);
	}
	if (WhirlSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WhirlSound, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
	bWhirling = true;
	UpdateHealthBar();
	GetWorldTimerManager().SetTimer(WhirlTimer, this, &ACombatCharacter::StopWhirling, 2.f);
	if (Attributes->GetHealth() == 0)
	{
		GetWorldTimerManager().SetTimer(DeathTimer, this, &ACombatCharacter::Die, 2.1f);
	}
}

void ACombatCharacter::ReceiveHealing(float Healing)
{
	if (Attributes->GetHealth() == Attributes->GetMaxHealth()) return;
	SpawnHealParticles();
	Attributes->ReceiveHealing(Healing);
	if (DamageIndicatorComponent)
	{
		DamageIndicatorComponent->DisplayHealing(Healing);
	}
	PlayHealSound();
	UpdateHealthBar();
}

void ACombatCharacter::IncreaseRage()
{
	Attributes->IncreaseRage();
	UpdateRageBar();
}

void ACombatCharacter::Die()
{
	Super::Die();
	RemoveEffectTags();
	Effects.Empty();
	UpdateEffectIcons();
	OnCharacterDeath.Broadcast(this);
}

void ACombatCharacter::HandleOnHover()
{
	if (bSelectable)
	{
		SetArrowToColor();
		ShowArrow();
		OnCharacterHover.Broadcast(this);
	}
}

void ACombatCharacter::HandleOnEndHover()
{
	HideArrow();
	if(bSelectable)
	{
		OnCharacterEndHover.Broadcast(this);
	}
}

void ACombatCharacter::HandleOnClicked()
{
	if(bSelectable)
	{
		OnCharacterClicked.Broadcast(this);
	}
	HideArrow();
}

void ACombatCharacter::AddEffect(UEffect* Effect)
{
	UEffect* ExistingEffectOfType = GetEffectOfType(Effect->EffectType);
	ShowSpecialInfoText(Effect->EffectName + TEXT("!"));
	if (ExistingEffectOfType && Effect->GetValue())
	{
		float NewValue = 0.f;
		FString AffectedAttribute = Effect->GetAffectedAttribute();
		switch (ExistingEffectOfType->TurnMode)
		{
		case EEffectTurnMode::EETM_EndOfTurn:
			NewValue = FMath::CeilToFloat(Avg<float>(ExistingEffectOfType->GetValue(), Effect->GetValue())); break;
		case EEffectTurnMode::EETM_BuffDebuff:
			if (!AffectedAttribute.IsEmpty())
			{
				NewValue = ExistingEffectOfType->GetValue() + Effect->GetValue();
				StatModifiers.Emplace(AffectedAttribute, NewValue);
			} break;
		default: break;
		}
		ExistingEffectOfType->SetValue(NewValue);
	}
	if (ExistingEffectOfType && Effect->GetDuration())
	{
		ExistingEffectOfType->SetDuration(ExistingEffectOfType->GetDuration() + Effect->GetDuration());
	}
	if (!ExistingEffectOfType)
	{
		Effects.Add(DuplicateObject<UEffect>(Effect, this));
		if (Effect->TurnMode == EEffectTurnMode::EETM_BuffDebuff)
		{
			StatModifiers.Emplace(Effect->GetAffectedAttribute(), Effect->GetValue());
		}
	}
	if(Effect->EffectSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Effect->EffectSound, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
	if (Effect->EffectType == EEffectType::EET_Defender)
	{
		Tags.AddUnique(FName("Defender"));
	}
	UpdateEffectIcons();
}

void ACombatCharacter::RemoveNegativeEffects()
{
	for (UEffect* Effect : Effects)
	{
		if (Effect->TurnMode == EEffectTurnMode::EETM_BuffDebuff && Effect->GetValue()<0 && !Effect->GetAffectedAttribute().IsEmpty())
		{
			StatModifiers.Remove(Effect->GetAffectedAttribute());
			Effects.Remove(Effect);
		}
		if (Effect->TurnMode == EEffectTurnMode::EETM_EndOfTurn && !Effect->bHeal)
		{
			Effects.Remove(Effect);
		}
		if (Effect->EffectType == EEffectType::EET_Deathwish)
		{
			Effects.Remove(Effect);
		}
	}
	UpdateEffectIcons();
}

void ACombatCharacter::RemoveEffectTags()
{
	for (UEffect* Effect : Effects)
	{
		if (Tags.Contains(FName(Effect->EffectName)))
		{
			Tags.Remove(FName(Effect->EffectName));
		}
	}
}

void ACombatCharacter::AddStatModifiers()
{
	if (Effects.IsEmpty()) return;
	for (UEffect* Effect : Effects)
	{
		if (Effect->TurnMode == EEffectTurnMode::EETM_BuffDebuff && !Effect->GetAffectedAttribute().IsEmpty())
		{
			StatModifiers.Emplace(Effect->GetAffectedAttribute(), Effect->GetValue());
		}
	}
}

void ACombatCharacter::RemoveStatModifiers()
{
	if (StatModifiers.IsEmpty()) return;
	for (TPair<FString, int16> Modifier : StatModifiers)
	{
		StatModifiers.Remove(Modifier.Key);
	}
}

void ACombatCharacter::Dodge()
{
	ShowSpecialInfoText(TEXT("UNIK!"));
	ChangeFlipbook(TEXT("Dodge"));
	PlayDodgeSound();
}

void ACombatCharacter::AttackMissed()
{
	PlayAttackMissedSound();
	ShowSpecialInfoText(TEXT("PUD\u0141O!"));
}

void ACombatCharacter::ReceiveCrit()
{
	ShowSpecialInfoText(TEXT("KRYTYK!"));
	if (ActorHasTag(FName("MainCharacter")))
	{
		GetMad();
	}
}

void ACombatCharacter::GetMad()
{
	if (!ActorHasTag(FName("MainCharacter"))) return;
	ShowSpecialInfoText(TEXT("Wkurw!"));
	ChangeFlipbook(TEXT("Rage"));
	IncreaseRage();
	if (Attributes->GetRage() == Attributes->GetMaxRage())
	{
		Ult->bUsable = true;
	}
	PlayRageSound();
}

void ACombatCharacter::GetStunned()
{
	Tags.Add(FName("Stunned"));
	ShowSpecialInfoText(TEXT("Og\u0142uszenie!"));
	ChangeFlipbook(TEXT("Stunned"));
}

void ACombatCharacter::StopWhirling()
{
	bWhirling = false;
	GetSprite()->SetRelativeRotation(FRotator::ZeroRotator);
}

void ACombatCharacter::ReturnFromStun()
{
	Tags.Remove(FName("Stunned"));
	ChangeFlipbook(TEXT("IdleCombat"));
}

UEffect* ACombatCharacter::GetEffectOfType(EEffectType Type)
{
	if (Effects.IsEmpty()) return nullptr;
	for (UEffect* Effect : Effects)
	{
		if (Effect->EffectType == Type)
		{
			return Effect;
		}
	}
	return nullptr;
}

void ACombatCharacter::UseMana(float Value)
{
	float NewMana = FMath::Clamp(Attributes->GetMana() - Value, 0.f, Attributes->GetMaxMana());
	Attributes->SetMana(NewMana);
	UpdateManaBar();
}

template<typename T>
inline T ACombatCharacter::Avg(T first, T second)
{
	return (first + second) / 2;
}

void ACombatCharacter::PlayHealSound()
{
	if (HealSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HealSound, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
}

void ACombatCharacter::PlayDodgeSound()
{
	if (DodgeSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DodgeSound, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
}

void ACombatCharacter::PlayRageSound()
{
	if (Attributes->GetRage() < Attributes->GetMaxRage() && RageSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RageSound, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
	else if (Attributes->GetRage() == Attributes->GetMaxRage() && MaxRageSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, MaxRageSound, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
	else return;
}

void ACombatCharacter::PlayAttackMissedSound()
{
	if (AttackMissedSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AttackMissedSound, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
}

void ACombatCharacter::SpawnHealParticles()
{
	if (HealVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HealVFX, ParticleSpawner->GetComponentLocation());
	}
}

void ACombatCharacter::SpawnEffectParticles(UEffect* Effect)
{
	if (Effect->EffectVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Effect->EffectVFX, ParticleSpawner->GetComponentLocation());
	}
}
