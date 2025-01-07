// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMechanics/Quests/Quest.h"
#include "GameMechanics/Quests/ObjectiveCollection.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "Kismet/GameplayStatics.h"

AQuest::AQuest()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AQuest::BeginPlay()
{
	Super::BeginPlay();
	RootObjectiveCollection = InitializeRoot();
	InitializeObjectives();
}

void AQuest::StartQuest()
{
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(this));
	if (CurrentGameMode == nullptr) return;
	if(RootObjectiveCollection)
	{
		RootObjectiveCollection->ActivateObjective(true);
	}
	CurrentGameMode->Quests.AddUnique(this);
}

UObjectiveCollection* AQuest::InitializeRoot_Implementation()
{
	return nullptr;
}

void AQuest::InitializeObjectives_Implementation()
{
}

void AQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

