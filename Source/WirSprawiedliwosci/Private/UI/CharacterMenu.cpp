// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterMenu.h"
#include "UI/MenuCharacterIcon.h"
#include "UI/AttributesMenu.h"
#include "UI/PartyMenu.h"
#include "UI/SkillMenu.h"
#include "Components/TextBlock.h"
#include "Characters/SpriteCharacter.h"
#include "Characters/Ally.h"
#include "GameMechanics/AttributeComponent.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "Kismet/GameplayStatics.h"

void UCharacterMenu::SetCharacterIcons()
{
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode == nullptr) return;
	TArray<UMenuCharacterIcon*> CharacterIcons = GetCharacterIconsAsArray();
	for (int i = 0; i < CharacterIcons.Num(); i++)
	{
		if (i < CurrentGameMode->AvailableAllies.Num())
		{
			CharacterIcons[i]->SetNewCharacter(CurrentGameMode->AvailableAllies[i]);
			CharacterIcons[i]->OnCharacterIconClicked.AddUniqueDynamic(this, &UCharacterMenu::HandleCharacterIconClicked);
		}
		else
		{
			CharacterIcons[i]->SetNewCharacter(nullptr);
			CharacterIcons[i]->OnCharacterIconClicked.Clear();
		}
	}
}

void UCharacterMenu::HandleCharacterIconClicked(ASpriteCharacter* ClickedCharacter)
{
	if (ClickedCharacter == nullptr) return;
	SetCurrentAlly(Cast<AAlly>(ClickedCharacter));
}

void UCharacterMenu::SetCurrentAlly(AAlly* NewAlly)
{
	if (NewAlly == nullptr) return;
	CurrentAlly = NewAlly;
	CurrentCharacterIcon->SetNewCharacter(CurrentAlly);
	CurrentCharacterName->SetText(FText::FromString(CurrentAlly->CharacterName));
	CurrentCharacterExp->SetText(FText::FromString(TEXT("poz. ") + FString::FromInt(CurrentAlly->Attributes->GetLevel()) 
								+ TEXT(" | ") + FString::FromInt(CurrentAlly->GetExperience()) + TEXT("/") +
								FString::FromInt(CurrentAlly->GetExpToNextLevel()) + TEXT(" PD")));

	if (SkillsMenu)
	{
		SkillsMenu->SetNewAlly(CurrentAlly);
	}
	if (AttributesMenu)
	{
		AttributesMenu->SetNewAlly(CurrentAlly);
	}
	if (PartyMenu)
	{
		PartyMenu->CurrentAlly = CurrentAlly;
	}
}

TArray<UMenuCharacterIcon*> UCharacterMenu::GetCharacterIconsAsArray()
{
	TArray<UMenuCharacterIcon*> CharacterIcons;
	CharacterIcons.AddUnique(CharacterIcon1);
	CharacterIcons.AddUnique(CharacterIcon2);
	CharacterIcons.AddUnique(CharacterIcon3);
	CharacterIcons.AddUnique(CharacterIcon4);
	CharacterIcons.AddUnique(CharacterIcon5);
	CharacterIcons.AddUnique(CharacterIcon6);
	CharacterIcons.AddUnique(CharacterIcon7);
	CharacterIcons.AddUnique(CharacterIcon8);
	return CharacterIcons;
}
