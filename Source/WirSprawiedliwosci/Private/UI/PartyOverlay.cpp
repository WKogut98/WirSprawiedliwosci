#include "UI/PartyOverlay.h"
#include "UI/CharacterIcon.h"
#include "Characters/Ally.h"
#include "Kismet/GameplayStatics.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"

void UPartyOverlay::InitializePartyIcons()
{
	UWorld* World = GetWorld();
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(World);
	if (!World || !GameMode) return;
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(GameMode);
	if (!CurrentGameMode) return;
	TArray<UCharacterIcon*> PartyIcons = GetIconsAsArray();
	for (int i = 0; i < 4; i++)
	{
		if (i < CurrentGameMode->AlliedParty.Num() && CurrentGameMode->AlliedParty[i])
		{
			PartyIcons[i]->SetNewCharacter(CurrentGameMode->AlliedParty[i]);
		}
		else
		{
			PartyIcons[i]->SetNewCharacter(nullptr);
		}
	}
}

TArray<UCharacterIcon*> UPartyOverlay::GetIconsAsArray()
{
	TArray<UCharacterIcon*> PartyIcons;
	PartyIcons.Add(PartyIcon4);
	PartyIcons.Add(PartyIcon3);
	PartyIcons.Add(PartyIcon2);
	PartyIcons.Add(PartyIcon1);
	return PartyIcons;
}
