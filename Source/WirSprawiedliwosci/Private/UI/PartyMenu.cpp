// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PartyMenu.h"
#include"UI/MenuCharacterIcon.h"
#include "UI/DefaultHUD.h"
#include "UI/PartyOverlay.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/SpriteCharacter.h"
#include "Characters/Ally.h"
#include "Characters/MainCharacter.h"

void UPartyMenu::UpdatePartyIcons()
{
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode == nullptr || CurrentGameMode->AlliedParty.IsEmpty()) return;
	TArray<UMenuCharacterIcon*> PartyIcons = GetPartyIconsAsArray();
	for (int i = 0; i < PartyIcons.Num(); i++)
	{
		if (i < CurrentGameMode->AlliedParty.Num())
		{
			PartyIcons[i]->SetNewCharacter(CurrentGameMode->AlliedParty[i]);
		}
		else
		{
			PartyIcons[i]->SetNewCharacter(nullptr);
		}
		if (i != 0)
		{
			PartyIcons[i]->OnCharacterIconClicked.AddUniqueDynamic(this, &UPartyMenu::SwapCharacter);
		}
	}
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD && CurrentHUD->PartyOverlay)
	{
		CurrentHUD->PartyOverlay->InitializePartyIcons();
	}
}

void UPartyMenu::SwapCharacter(ASpriteCharacter* ClickedCharacter)
{
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode == nullptr || CurrentGameMode->AlliedParty.IsEmpty()) return;
	if (CurrentAlly == nullptr || !bCanSwapCharacters) return;
	AAlly* ClickedAlly = Cast<AAlly>(ClickedCharacter);
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (MainCharacter == nullptr) return;
	if (CurrentGameMode->AlliedParty.Contains(CurrentAlly)) return;
	if (ClickedAlly == nullptr)
	{
		CurrentGameMode->AlliedParty.AddUnique(CurrentAlly);
		CurrentAlly->Rank = CurrentGameMode->AlliedParty.Find(CurrentAlly)+1;
		CurrentAlly->SetActorLocation(MainCharacter->GetActorLocation() - ((CurrentAlly->Rank - 1) * 80.f));
	}
	else
	{
		int32 NewRank = ClickedAlly->Rank;
		ClickedAlly->Rank = 0;
		CurrentAlly->Rank = NewRank;
		CurrentGameMode->AlliedParty.Remove(ClickedAlly);
		CurrentGameMode->AlliedParty.Insert(CurrentAlly, FMath::Clamp(NewRank - 1, 1, 3));
		FVector Location = CurrentAlly->GetActorLocation();
		CurrentAlly->SetActorLocation(FVector::ZeroVector);
		ClickedAlly->SetActorLocation(Location);
		CurrentAlly->SetActorLocation(MainCharacter->GetActorLocation() - ((CurrentAlly->Rank - 1) * 80.f));
	}
	UpdatePartyIcons();
	bCanSwapCharacters = false;
}

void UPartyMenu::ActivateSwapping()
{
	bCanSwapCharacters = true;
}

TArray<UMenuCharacterIcon*> UPartyMenu::GetPartyIconsAsArray()
{
	TArray<UMenuCharacterIcon*> PartyIcons;
	PartyIcons.Add(PartyIcon1);
	PartyIcons.Add(PartyIcon2);
	PartyIcons.Add(PartyIcon3);
	PartyIcons.Add(PartyIcon4);
	return PartyIcons;
}
