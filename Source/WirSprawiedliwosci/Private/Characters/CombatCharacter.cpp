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
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ACombatCharacter::ACombatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CharacterWidgetComponent = CreateDefaultSubobject<UCharacterWidgetComponent>(TEXT("Characer Widget"));
	CharacterWidgetComponent->SetupAttachment(GetRootComponent());

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
	UE_LOG(LogTemp, Log, TEXT("Starting turn for character: %s"), *GetName());
	Turn++;
	ApplyEffectsPreTurn();
}

void ACombatCharacter::EndTurn()
{
	ApplyEffectsPostTurn();
	UpdateEffectIcons();
}

void ACombatCharacter::ApplyEffectsPreTurn()
{
	for (UEffect* Effect : Effects)
	{
		FString AffectedAttribute = Effect->GetAffectedAttribute();
		if (Effect->TurnMode == EEffectTurnMode::EETM_BuffDebuff && !AffectedAttribute.IsEmpty())
		{
			uint8 NewStatValue = *Attributes->Stats.Find(AffectedAttribute) + Effect->GetValue();
			Attributes->Stats.Emplace(AffectedAttribute, NewStatValue);
			Effect->bApplied = true;
		}
		if (Effect->TurnMode == EEffectTurnMode::EETM_Passive)
		{
			Effect->bApplied = true;
		}
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
			uint8 NewStatValue = *Attributes->Stats.Find(AffectedAttribute) - Effect->GetValue();
			Attributes->Stats.Emplace(AffectedAttribute, NewStatValue);
			Effect->SetDuration(Effect->GetDuration() - 1);
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
		CharacterWidgetComponent->SetSpecialInfoText(SpecialText);
		CharacterWidgetComponent->ShowSpecialInfoText();
		GetWorldTimerManager().SetTimer(SpecialTextTimer, CharacterWidgetComponent, &UCharacterWidgetComponent::HideSpecialInfoText, 1.5f);
	}
}

void ACombatCharacter::GetHit(float Damage)
{
	if (GetEffectOfType(EEffectType::EET_Deathwish))
	{
		Damage *= 2;
	}
	Attributes->ReceiveDamage(Damage);

	//efekty
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ParticleSpawner->GetComponentLocation(), GetActorRotation());
	}
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ParticleSpawner->GetComponentLocation(), ParticleSpawner->GetComponentRotation());
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
				uint8 NewStatValue = *Attributes->Stats.Find(AffectedAttribute) + Effect->GetValue();
				Attributes->Stats.Emplace(AffectedAttribute, NewStatValue);
			}
			NewValue = ExistingEffectOfType->GetValue() + Effect->GetValue(); break;
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
			uint8 NewValue = *Attributes->Stats.Find(Effect->GetAffectedAttribute()) - Effect->GetValue();
			Attributes->Stats.Emplace(Effect->GetAffectedAttribute(), NewValue);
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