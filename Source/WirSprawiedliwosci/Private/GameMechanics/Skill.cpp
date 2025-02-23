// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMechanics/Skill.h"
#include "GameMechanics/AttributeComponent.h"
#include "Characters/CombatCharacter.h"
#include "Characters/MainCharacter.h"
#include "PaperFlipbookComponent.h"
//#include "Characters/CharacterStates.h"
#include "GameMechanics/Effect.h"
#include "Items/Item.h"
#include "Kismet/GameplayStatics.h"

USkill::USkill()
{
	Description = TEXT("");
}

void USkill::UseSkill(ACombatCharacter* NewSource, TArray<ACombatCharacter*> NewTargets)
{
	if (NewSource && !NewTargets.IsEmpty())
	{
		Source = NewSource;
		Targets = NewTargets;
		ApplySpecialProperty();
		SetSourceFlipbook();
		PlaySkillSound();
		if(Source)
		{
			Source->GetWorldTimerManager().SetTimer(SkillDelayTimer, this, &USkill::ExecuteUseSkill, AnimDuration);
		}
	}
}

void USkill::ExecuteUseSkill()
{
	if (ManaCost > 0.f)
	{
		Source->UseMana(ManaCost);
		//Source->UpdateManaBar();
	}
	if (NumberOfTargets == 0 && bRemovesNegativeEffects)
	{
		Source->RemoveNegativeEffects();
	}
	if (SelfHeal > 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s self-healed for %d"), *Source->GetName(), SelfHeal);
		Source->ReceiveHealing(SelfHeal);
		Source->UpdateHealthBar();
	}
	if (SelfDamage > 0.f)
	{
		Source->GetHit(SelfDamage);
		Source->UpdateHealthBar();
	}
	if (!SelfEffects.IsEmpty())
	{
		for (UEffect* SelfEffect : SelfEffects)
		{
			Source->AddEffect(SelfEffect);
		}
		Source->UpdateEffectIcons();
	}
	for (ACombatCharacter* Target : Targets)
	{
		if (Target && (SkillType == ESkillType::EST_Attack || SkillType == ESkillType::EST_UltAttack || SkillType == ESkillType::EST_AttackAlly))
		{
			UseOffensiveSkill(Target);
		}
		else if ((Target) && (SkillType == ESkillType::EST_Heal || SkillType == ESkillType::EST_Support || SkillType == ESkillType::EST_UltSupport))
		{
			UseSupportSkill(Target);
		}
	}
}

void USkill::ApplySpecialProperty()
{
	AAlly* Ally = Cast<AAlly>(Source);
	switch (SpecialProperty)
	{
	case ESkillSpecialProperty::EST_UsesRandomFood:
		if (Ally)
		{
			Ally->UseRandomFood();
		}
		break;
	case ESkillSpecialProperty::EST_MakesTargetInvisible:
		if (Targets[0])
		{
			Targets[0]->ChangeFlipbook(FString("Blank"));
		}
		break;
	default:
		break;
	}
}

void USkill::UseSupportSkill(ACombatCharacter* Target)
{
	if (MinValue && MaxValue)
	{
		uint8 HealingProficiency = *Source->Attributes->Stats.Find(TEXT("Bieg\u0142o\u015b\u0107 Lecznicza"));
		float Value = CalculateValue(Target);
		Value = Value + FMath::Floor(0.05 * Value * HealingProficiency);
		Target->ReceiveHealing(Value);
		//Target->UpdateHealthBar();
	}

	if (InflictedEffect)
	{
		Target->AddEffect(InflictedEffect);
		Target->UpdateEffectIcons();
	}

	if (bRemovesNegativeEffects)
	{
		Target->RemoveNegativeEffects();
	}

	if (SkillType == ESkillType::EST_UltSupport && Source->ActorHasTag(FName("MainCharacter")))
	{
		Source->Attributes->SetRage(0.f);
		Source->UpdateRageBar();
		bUsable = false;
		//Source->UpdateRageBar();
	}
}

void USkill::UseOffensiveSkill(ACombatCharacter* Target)
{
	float Accuracy = BaseAccuracy + *Source->Attributes->Stats.Find(TEXT("Celno\u015b\u0107")) - 10.f;
	if (Source->StatModifiers.Contains(TEXT("Celno\u015b\u0107")))
	{
		int16 AccuracyModifier = *Source->StatModifiers.Find(TEXT("Celno\u015b\u0107"));
		if (AccuracyModifier < 0)
		{
			AccuracyModifier = FMath::Clamp(AccuracyModifier, 0 - (*Source->StatModifiers.Find(TEXT("Celno\u015b\u0107"))), 0.f);
		}
		Accuracy += AccuracyModifier;
	}
	Accuracy = FMath::Clamp(Accuracy, 0.f, 100.f);
	if (Target == nullptr || Target->Attributes == nullptr) return;
	if (DetermineSuccess(Accuracy) || SkillType==ESkillType::EST_AttackAlly)
	{
		if (DetermineSuccess(*Target->Attributes->Stats.Find(TEXT("Unik"))) && SkillType != ESkillType::EST_UltSupport && 
			SkillType != ESkillType::EST_UltAttack && !Target->ActorHasTag(FName("Stunned")))
		{
			Target->Dodge();
			if (Source->ActorHasTag(FName("MainCharacter")))
			{
				Source->GetMad();
				//Source->UpdateRageBar();
			}
		}
		else
		{
			if (MaxValue && MinValue)
			{
				float Value = CalculateValue(Target);
				uint8 Power = *Source->Attributes->Stats.Find(TEXT("Zdolno\u015b\u0107 do Ludob\u00f3jstwa"));
				if (Source->StatModifiers.Contains(TEXT("Zdolno\u015b\u0107 do Ludob\u00f3jstwa")))
				{
					int16 PowerModifier = *Source->StatModifiers.Find(TEXT("Zdolno\u015b\u0107 do Ludob\u00f3jstwa"));
					if (PowerModifier < 0)
					{
						PowerModifier = FMath::Clamp(PowerModifier, 0-Power, 0);
					}
					Power += PowerModifier;
				}
				uint8 Defence = *Target->Attributes->Stats.Find(TEXT("Obrona"));
				if (Target->StatModifiers.Contains(TEXT("Obrona")))
				{
					int16 DefenceModifier = *Target->StatModifiers.Find(TEXT("Obrona"));
					if (DefenceModifier < 0)
					{
						DefenceModifier = FMath::Clamp(DefenceModifier, 0-Defence, 0);
					}
					Defence += DefenceModifier;
				}
				Value = Value + FMath::Floor(0.05 * Value * Power) - FMath::Floor(0.05 * Value * Defence);

				if (SkillName == TEXT("Wir"))
				{
					Target->GetHitVortex(Value);
				}
				else
				{
					Target->GetHit(Value, DamageType);
				}
				if (Target && Target->ActorHasTag(FName("Kontra")))
				{
					Target->ChangeFlipbook(TEXT("Counter"));
					uint8 TargetPower = *Target->Attributes->Stats.Find(TEXT("Zdolno\u015b\u0107 do Ludob\u00f3jstwa"));
					if (Target->StatModifiers.Contains(TEXT("Zdolno\u015b\u0107 do Ludob\u00f3jstwa")))
					{
						int16 PowerModifier = *Target->StatModifiers.Find(TEXT("Zdolno\u015b\u0107 do Ludob\u00f3jstwa"));
						if (PowerModifier < 0)
						{
							PowerModifier = FMath::Clamp(PowerModifier, 0-TargetPower, 0);
						}
						TargetPower += PowerModifier;
					}
					uint8 SourceDefence = *Source->Attributes->Stats.Find(TEXT("Obrona"));
					if (Source->StatModifiers.Contains(TEXT("Obrona")))
					{
						int16 DefenceModifier = *Source->StatModifiers.Find(TEXT("Obrona"));
						if (DefenceModifier < 0)
						{
							DefenceModifier = FMath::Clamp(DefenceModifier, 0-SourceDefence, 0);
						}
						SourceDefence += DefenceModifier;
					}
					float CounterValue = 2.f;//Value * 0.3f;
					Source->GetHit(CounterValue + FMath::Floor(0.05 * CounterValue * TargetPower));
					Source->ShowSpecialInfoText(TEXT("Skontrowano!"));
				}
			}
			if (DetermineSuccess(StunRate - *Target->Attributes->Resistances.Find(TEXT("Og\u0142uszenie"))))
			{
				Target->GetStunned();
			}
			if (InflictedEffect && !InflictedEffect->EffectName.IsEmpty() && 
				Target->Attributes->Resistances.Find(InflictedEffect->EffectName))
			{
				bool bResisted = DetermineSuccess(100 - *Target->Attributes->Resistances.Find(InflictedEffect->EffectName));
				if (bResisted)
				{
					Target->ShowSpecialInfoText(TEXT("Odporno\u015b\u0107!"));
				}
				else
				{
					Target->AddEffect(InflictedEffect);
				}
				Target->UpdateEffectIcons();
			}
		}
	}
	else
	{
		Target->AttackMissed();
		if (Source->ActorHasTag(FName("MainCharacter")))
		{
			Source->GetMad();
			Source->UpdateRageBar();
		}
	}
	if (bRemovesNegativeEffects)
	{
		Source->RemoveNegativeEffects();
	}
	if (SkillType == ESkillType::EST_UltAttack && Source->ActorHasTag(FName("MainCharacter")))
	{
		Source->Attributes->SetRage(0.f);
		Source->UpdateRageBar();
		bUsable = false;
	}
}

void USkill::LevelUp()
{
}

void USkill::BuildDescription()
{
	//jeœli zu¿ywa manê
	if (ManaCost)
	{
		Description += TEXT("Koszt Many: ") + FString::FromInt(ManaCost) + LINE_TERMINATOR;
	}
	//obra¿enia lub leczenie
	if (MinValue && MaxValue)
	{
		if (!SpecificTargetName.IsEmpty())
		{
			Description += TEXT("Na ") + GetSpecificTargetNameForDescription();
		}
		FString HealOrDmg;
		if (SkillType == ESkillType::EST_Attack || SkillType == ESkillType::EST_UltAttack || SkillType == ESkillType::EST_AttackAlly)
		{
			HealOrDmg = TEXT("Obra\u017cenia: ");
		}

		if (SkillType == ESkillType::EST_Heal || SkillType == ESkillType::EST_Support || SkillType==ESkillType::EST_UltSupport)
		{
			HealOrDmg = TEXT("Leczenie: ");
		}
		HealOrDmg += FString::FromInt(MinValue) + TEXT("-") + FString::FromInt(MaxValue);
		Description += HealOrDmg + LINE_TERMINATOR;
	}

	//nak³adanie statusu na cel
	if (InflictedEffect)
	{
		if (!SpecificTargetName.IsEmpty())
		{
			Description += TEXT("Na ") + GetSpecificTargetNameForDescription();
		}
		BuildDescriptionForEffect(InflictedEffect);
	}

	//nak³adanie statusu na siebie
	if (!SelfEffects.IsEmpty())
	{
		Description += TEXT("Na siebie: ");
		for (UEffect* SelfEffect : SelfEffects)
		{
			BuildDescriptionForEffect(SelfEffect);
		}
	}

	//zadanie sobie obra¿eñ
	if (SelfDamage)
	{
		Description += TEXT("Zadaj sobie ") 
			+ FString::FromInt(SelfDamage) + TEXT(" punkt\u00f3w obra\u017ce\u0144") + LINE_TERMINATOR;
	}

	//uleczenie samego siebie
	if (SelfHeal)
	{
		Description += TEXT("Na siebie: Ulecz ") + FString::FromInt(SelfHeal)
			+ TEXT(" punkt\u00f3w obra\u017ce\u0144") + LINE_TERMINATOR;
	}

	//jeœli usuwa negatywne statusy
	if (bRemovesNegativeEffects)
	{
		Description += TEXT("Usu\u0144 wszystkie negatywne statusy\n");
	}

	//celnoœæ
	if (BaseAccuracy)
	{
		Description += TEXT("Celno\u015b\u0107 bazowa: ") + FString::FromInt(BaseAccuracy) + TEXT("%") + LINE_TERMINATOR;
	}

	//krytyki
	if (BaseCritRate)
	{
		Description += TEXT("Szansa na krytyk: ") + FString::FromInt(BaseCritRate) + TEXT("%") + LINE_TERMINATOR;
	}

	//og³uszenie
	if (StunRate)
	{
		Description += TEXT("Szansa na og\u0142uszenie: ") + FString::FromInt(StunRate) + TEXT("%") + LINE_TERMINATOR;
	}

	//a po kim ty taki chitry??
	if (SpecialProperty==ESkillSpecialProperty::EST_UsesRandomFood)
	{
		Description += TEXT("Zu\u017cyj losowy przedmiot z kategorii jedzenie\n");
	}
}

void USkill::BuildDescriptionForEffect(UEffect* Effect)
{
	if (Effect)
	{
		FString EffectName = Effect->EffectName;
		Description += EffectName + TEXT(" ");
		if (Effect->GetValue()) //wartoœæ leczenia/obra¿eñ
		{
			Description += FString::FromInt(abs(Effect->GetValue()));
		}
		else if (Effect->GetValue() && Effect->TurnMode==EEffectTurnMode::EETM_BuffDebuff) //modyfikator statystyk
		{
			if (Effect->GetValue() > 0.f)
			{
				Description += TEXT("+");
			}
			Description += FString::FromInt(Effect->GetValue()) + TEXT(" ")
				+ Effect->GetAffectedAttribute();
		}
		if (Effect->GetDuration()) //przez ile tur
		{
			FString Turns = Effect->GetDuration() == 1 ? TEXT(" tur\u0119.") : TEXT(" tur(y).");
			Description += TEXT(" przez ") + FString::FromInt(Effect->GetDuration())
				+ Turns;
		}
		Description += LINE_TERMINATOR;
	}
}

float USkill::CalculateValue(ACombatCharacter* Target)
{
	//zwiêkszamy szansê na krytyk o szczêœcie bohatera
	uint8 Luck = *Source->Attributes->Stats.Find(TEXT("Szcz\u0119\u015bcie"));
	float CritRate = BaseCritRate + Luck - 10.f; //(uint8)FMath::Floor(BaseCritRate * 0.05 * Luck)
	CritRate = FMath::Clamp(CritRate, 0.f, 100.f);
	float Value = FMath::RandRange(MinValue, MaxValue);
	if (DetermineSuccess(CritRate))
	{
		Value *= 2;
		Target->ReceiveCrit();
	}
	return Value;
}


bool USkill::DetermineSuccess(float Chance)
{
	float Random = FMath::RandRange(0.f, 100.f);
	if (Random <= Chance)
	{
		return true;
	}
	return false;
}

FString USkill::GetSpecificTargetNameForDescription()
{
	if (SpecificTargetName.Equals(TEXT("Janusz")))
	{
		return TEXT("Janusza: ");
	}
	if (SpecificTargetName.Equals(TEXT("Pjoter")))
	{
		return TEXT("Pjotera: ");
	}
	if (SpecificTargetName.Equals(TEXT("\u015awagier")))
	{
		return TEXT("\u015awagra: ");
	}
	if (SpecificTargetName.Equals(TEXT("Halyna")))
	{
		return TEXT("Halyn\u0119: ");
	}
	if (SpecificTargetName.Equals(TEXT("Ksiunc Stasiek")))
	{
		return TEXT("Ksiendza Sta\u015aka: ");
	}
	if (SpecificTargetName.Equals(TEXT("Seba")))
	{
		return TEXT("Seb\u0119");
	}
	if (SpecificTargetName.Equals(TEXT("Karyna")))
	{
		return TEXT("Karyn\u0119: ");
	}
	if (SpecificTargetName.Equals(TEXT("Talarek")))
	{
		return TEXT("Talarka: ");
	}
	return TEXT("");
}

void USkill::SetSourceFlipbook()
{
	if (Source && !Source->GetActionSprites().IsEmpty())
	{
		Source->ChangeFlipbook(ActionKey);
	}
}

void USkill::PlaySkillSound()
{
	if (Source && SkillSound)
	{
		UGameplayStatics::PlaySoundAtLocation(Source, SkillSound, Source->GetActorLocation());
	}
}
