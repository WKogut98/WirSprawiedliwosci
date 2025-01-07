#include "UI/AttributesMenu.h"
#include "UI/LevelUpButton.h"
#include "Characters/Ally.h"
#include "GameMechanics/AttributeComponent.h"
#include "Components/TextBlock.h"

void UAttributesMenu::SetNewAlly(AAlly* NewAlly)
{
	if (NewAlly)
	{
		CurrentAlly = NewAlly;
		UpdateElements();
	}
	
}

void UAttributesMenu::UpdateElements()
{
	UpdateAttributes();
	UpdateLevelUpButtons();
}

void UAttributesMenu::UpdateAttributes()
{
	if (CurrentAlly == nullptr) return;
	Vitality->SetText(FText::AsNumber(*CurrentAlly->Attributes->Stats.Find(TEXT("Witalno\u015b\u0107"))));
	Magic->SetText(FText::AsNumber(*CurrentAlly->Attributes->Stats.Find(TEXT("Magia"))));
	Genocide->SetText(FText::AsNumber(*CurrentAlly->Attributes->Stats.Find(TEXT("Zdolno\u015b\u0107 do Ludob\u00f3jstwa"))));
	Healing->SetText(FText::AsNumber(*CurrentAlly->Attributes->Stats.Find(TEXT("Bieg\u0142o\u015b\u0107 Lecznicza"))));
	Defence->SetText(FText::AsNumber(*CurrentAlly->Attributes->Stats.Find(TEXT("Obrona"))));
	Dodge->SetText(FText::AsNumber(*CurrentAlly->Attributes->Stats.Find(TEXT("Unik"))));
	Speed->SetText(FText::AsNumber(*CurrentAlly->Attributes->Stats.Find(TEXT("Narwano\u015b\u0107"))));
	Luck->SetText(FText::AsNumber(*CurrentAlly->Attributes->Stats.Find(TEXT("Szcz\u0119\u015bcie"))));
	Accuracy->SetText(FText::AsNumber(*CurrentAlly->Attributes->Stats.Find(TEXT("Celno\u015b\u0107"))));

	StunResistance->SetText(FText::FromString(FString::FromInt(*CurrentAlly->Attributes->Resistances.Find(TEXT("Og\u0142uszenie"))) + TEXT("%")));
	OppressionResistance->SetText(FText::FromString(FString::FromInt(*CurrentAlly->Attributes->Resistances.Find(TEXT("Ciemi\u0119\u017cenie"))) + TEXT("%")));
	StinkResistance->SetText(FText::FromString(FString::FromInt(*CurrentAlly->Attributes->Resistances.Find(TEXT("Smrud"))) + TEXT("%")));
	DisgraceResistance->SetText(FText::FromString(FString::FromInt(*CurrentAlly->Attributes->Resistances.Find(TEXT("Kompromitacja Cwela"))) + TEXT("%")));
	DrunkResistance->SetText(FText::FromString(FString::FromInt(*CurrentAlly->Attributes->Resistances.Find(TEXT("Najebanie"))) + TEXT("%")));

	AttributePoints->SetText(FText::FromString(TEXT("Punkty Atrybut\u00f3w: ") + FString::FromInt(CurrentAlly->AttributePoints)));
}

void UAttributesMenu::UpdateLevelUpButtons()
{
	TArray<ULevelUpButton*> LevelUpButtons = GetLevelUpButtonsAsArray();
	if (LevelUpButtons.IsEmpty()) return;
	for (ULevelUpButton* Button : LevelUpButtons)
	{
		Button->Ally = CurrentAlly;
		Button->OnStatLeveledUp.AddUniqueDynamic(this, &UAttributesMenu::UpdateElements);
		if (CurrentAlly->AttributePoints < 1)
		{
			Button->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			Button->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

TArray<ULevelUpButton*> UAttributesMenu::GetLevelUpButtonsAsArray()
{
	TArray<ULevelUpButton*> LevelUpButtons;
	LevelUpButtons.Add(LevelUpBtnVitality);
	LevelUpButtons.Add(LevelUpBtnMagic);
	LevelUpButtons.Add(LevelUpBtnGenocide);
	LevelUpButtons.Add(LevelUpBtnHealing);
	LevelUpButtons.Add(LevelUpBtnDefence);
	LevelUpButtons.Add(LevelUpBtnDodge);
	LevelUpButtons.Add(LevelUpBtnSpeed);
	LevelUpButtons.Add(LevelUpBtnLuck);
	LevelUpButtons.Add(LevelUpBtnAccuracy);
	return LevelUpButtons;
}
