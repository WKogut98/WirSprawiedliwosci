// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMechanics/Quests/QuestObjective.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "Characters/Ally.h"
#include "Kismet/GameplayStatics.h"
#include "UI/DefaultHUD.h"

void UQuestObjective::ActivateObjective_Implementation(bool bShowOnScreen)
{
	if(bShowOnScreen)
	{
		PlayActivateSound();
	}
	ADefaultHUD* CurrentHUD = Cast <ADefaultHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (CurrentHUD && bShowOnScreen)
	{
		CurrentHUD->ShowInfoText(ActivateText + TEXT(": ") + ObjectiveName.ToString());
	}
	bActive = true;
}

void UQuestObjective::Complete()
{
	bCompleted = true;
	PlayCompletedSound();
	ADefaultHUD* CurrentHUD = Cast <ADefaultHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (CurrentHUD)
	{
		CurrentHUD->ShowInfoText(CompletedText + TEXT(": ") + ObjectiveName.ToString() + TEXT(" +") + FString::FromInt(Experience) + TEXT(" PD"));
	}
	AwardExperience();
	OnObjectiveCompleted.Broadcast(this);
}

void UQuestObjective::AwardExperience()
{
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode == nullptr || CurrentGameMode->AvailableAllies.IsEmpty()) return;
	for (AAlly* Ally : CurrentGameMode->AlliedParty)
	{
		if (CurrentGameMode->AlliedParty.Contains(Ally))
		{
			Ally->SetExperience(Ally->GetExperience() + Experience);
		}
		else
		{
			Ally->SetExperience(Ally->GetExperience() + (Experience / 2));
		}
	}
}

void UQuestObjective::PlayCompletedSound()
{
	if (CompletedSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), CompletedSound);
	}
}

void UQuestObjective::PlayActivateSound()
{
	if (ActivateSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ActivateSound);
	}
}
