#include "Items/Item.h"
#include "GameMechanics/SkillTypes.h"
#include "GameMechanics/Effect.h"
#include "GameMechanics/AttributeComponent.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "Characters/Ally.h"
#include "UI/DefaultHUD.h"
#include "UI/CharacterWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

void UItem::AddToInventory()
{
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	for (int i = 0; i < 15; i++)
	{
		if (i < CurrentGameMode->Inventory.Num())
		{
			if (CurrentGameMode->Inventory[i] && CurrentGameMode->Inventory[i]->GetClass() == GetClass() &&
				CurrentGameMode->Inventory[i]->Quantity < MaxQuantity)
			{
				int32 NewQuantity = CurrentGameMode->Inventory[i]->Quantity + Quantity;
				CurrentGameMode->Inventory[i]->Quantity = FMath::Clamp(NewQuantity, 0, MaxQuantity);
				return;
			}
		}
		else
		{
			Quantity = FMath::Clamp(Quantity, 0, MaxQuantity);
			CurrentGameMode->Inventory.Add(this);
			return;
		}
	}
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD)
	{
		CurrentHUD->ShowInfoText("Brak miejsca w ekwipunku!");
	}
}

void UItem::UseItem(AAlly* Ally)
{
	if (Ally == nullptr || !Ally->bCanUseItems) return;
	SpawnItemVFX(Ally);
	PlayUseSound();
	if (Healing)
	{
		Ally->ReceiveHealing(Healing);
		Ally->UpdateHealthBar();
	}
	if (Mana)
	{
		float NewMana = Ally->Attributes->GetMana() + Mana;
		NewMana = FMath::Clamp(NewMana, 0.f, Ally->Attributes->GetMaxMana());
		Ally->Attributes->SetMana(NewMana);
		Ally->UpdateManaBar();
	}
	if (Rage)
	{
		float NewRage = Ally->Attributes->GetRage() + Rage;
		NewRage = FMath::Clamp(NewRage, 0.f, Ally->Attributes->GetMaxRage());
		Ally->Attributes->SetRage(NewRage);
		Ally->UpdateRageBar();
	}
	if (InflictedEffectClass)
	{
		InflictedEffect = NewObject<UEffect>(GetOuter(), InflictedEffectClass);
		InflictedEffect->SetValue(InflictedEffectValue);
		InflictedEffect->SetDuration(InflictedEffectDuration);
		Ally->AddEffect(InflictedEffect);
	}
	if (GainedExp)
	{
		int32 AllyExp = Ally->GetExperience();
		Ally->SetExperience(AllyExp + GainedExp);
	}
	if (bRemovesNegativeEffects)
	{
		Ally->RemoveNegativeEffects();
	}
	ApplyStatBonuses(Ally);
	ApplyResistanceBonuses(Ally);
	ApplyDrunk(Ally);
	if (ItemType == EItemType::EIT_Alcohol)
	{
		Ally->RoundsDrank++;
	}
	if (bNeverDepletes)
		return;
	Quantity -= 1;
	if (Quantity <= 0)
	{
		ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (CurrentGameMode)
		{
			CurrentGameMode->Inventory.Remove(this);
		}
	}
}

void UItem::ApplyStatBonuses(AAlly* Ally)
{
	if (!StatBonuses.IsEmpty())
	{
		for (auto StatBonus : StatBonuses)
		{
			uint8 StatValue = *Ally->Attributes->Stats.Find(StatBonus.Key);
			int32 Bonus = StatBonus.Value;
			if (Bonus >= 0)
			{
				StatValue += Bonus;
			}
			else
			{
				StatValue -= Bonus;
			}
			Ally->Attributes->Stats.Emplace(StatBonus.Key, StatValue);
		}
	}
}

void UItem::ApplyResistanceBonuses(AAlly* Ally)
{
	if (!ResistanceBonuses.IsEmpty())
	{
		for (auto ResistanceBonus : ResistanceBonuses)
		{
			uint8 ResistanceValue = *Ally->Attributes->Resistances.Find(ResistanceBonus.Key);
			int32 Bonus = ResistanceBonus.Value;
			if (Bonus >= 0)
			{
				ResistanceValue += Bonus;
			}
			else
			{
				ResistanceValue -= Bonus;
			}
			Ally->Attributes->Resistances.Emplace(ResistanceBonus.Key, ResistanceValue);
		}
	}
}

void UItem::ApplyDrunk(AAlly* Ally)
{
	if (ItemType != EItemType::EIT_Alcohol) return;
	int32 DrunkResistance = *Ally->Attributes->Resistances.Find(TEXT("Najebanie"));
	float Random = FMath::RandRange(0.f, 100.f);
	float Chance = 100.f + AlcoholVoltage - DrunkResistance + (0.12f * Ally->RoundsDrank * AlcoholVoltage);
	if (Random <= Chance)
	{
		UEffect* DrunkEffect = NewObject<UEffect>(this, DrunkEffectClass);
		DrunkEffect->SetValue(-2.f * (Ally->RoundsDrank+1) * 2);
		DrunkEffect->SetDuration(3);
		Ally->AddEffect(DrunkEffect);
	}
	if (Chance >= 100)
	{
		Ally->Attributes->SetHealth(0.f);
		Ally->UpdateHealthBar();
		Ally->Die();
	}

}

void UItem::BuildDescription()
{
	switch (ItemType)
	{
	case EItemType::EIT_Food:
		Description += TEXT("Jedzenie\n"); break;
	case EItemType::EIT_Alcohol:
		Description += TEXT("Alkohol\n"); break;
	case EItemType::EIT_Cigarettes:
		Description += TEXT("Papierosy\n"); break;
	case EItemType::EIT_Artifact:
		Description += TEXT("Artefakt\n"); break;
	default: break;
	}
	if (Healing)
	{
		Description += TEXT("Leczenie: ") + FString::FromInt(Healing) + LINE_TERMINATOR;
	}
	if (Mana)
	{
		Description += TEXT("Mana: ") + FString::FromInt(Mana) + LINE_TERMINATOR;
	}
	if (Rage)
	{
		Description += TEXT("Wkurw: ") + FString::FromInt(Rage) + LINE_TERMINATOR;
	}
	BuildDescriptionForStatBonuses();
	BuildDescriptionForResistanceBonuses();
	BuildDescriptionForEffect();
	if (GainedExp)
	{
		Description += TEXT("+") + FString::FromInt(GainedExp) + TEXT(" PD\n");
	}
	//jeœli usuwa negatywne statusy
	if (bRemovesNegativeEffects)
	{
		Description += TEXT("Usuwa wszystkie negatywne statusy\n");
	}
	if (AlcoholVoltage)
	{
		Description += TEXT("Wolta\u017c ") + FString::FromInt(AlcoholVoltage) + TEXT("%\n");
	}
}

void UItem::BuildDescriptionForStatBonuses()
{
	if (!StatBonuses.IsEmpty())
	{
		for (auto StatBonus : StatBonuses)
		{
			int32 Bonus = StatBonus.Value;
			FString BonusAsString = Bonus > 0 ? TEXT("+") + FString::FromInt(Bonus) : FString::FromInt(Bonus);
			Description += StatBonus.Key + TEXT(" ") + BonusAsString + LINE_TERMINATOR;
		}
	}
}

void UItem::BuildDescriptionForResistanceBonuses()
{
	if (!ResistanceBonuses.IsEmpty())
	{
		for (auto ResistanceBonus : ResistanceBonuses)
		{
			int32 Bonus = ResistanceBonus.Value;
			FString BonusAsString = Bonus > 0 ? TEXT("+") + FString::FromInt(Bonus) : FString::FromInt(Bonus);
			Description += TEXT("Odporno\u015b\u0107 na ") + ResistanceBonus.Key + TEXT(" ") + BonusAsString + LINE_TERMINATOR;
		}
	}
}

void UItem::BuildDescriptionForEffect()
{
	if (InflictedEffectClass)
	{
		Description += InflictedEffectName + TEXT(" ");
		if (InflictedEffectValue) //wartoœæ leczenia/obra¿eñ
		{
			Description += FString::FromInt(abs(InflictedEffectValue));
		}
		if (InflictedEffectDuration) //przez ile tur
		{
			FString Turns = InflictedEffectDuration == 1 ? TEXT(" tur\u0119.") : TEXT(" tur(y).");
			Description += TEXT(" przez ") + FString::FromInt(InflictedEffectDuration)
				+ Turns;
		}
		Description += LINE_TERMINATOR;
	}
}

void UItem::PlayUseSound()
{
	if (UseSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), UseSound);
	}
}

void UItem::SpawnItemVFX(AAlly* Ally)
{
	if (Ally && Ally->ParticleSpawner && ItemVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Ally, ItemVFX, Ally->ParticleSpawner->GetComponentLocation());
	}
}
