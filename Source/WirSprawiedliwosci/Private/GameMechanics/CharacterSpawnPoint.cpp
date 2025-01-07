// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMechanics/CharacterSpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"

void ACharacterSpawnPoint::BeginPlay()
{
	ADefaultGameMode* CurrentGameMode=Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(this));
	if (CurrentGameMode)
	{
		CurrentGameMode->AllySpawnPoints.AddUnique(this);
	}
}
