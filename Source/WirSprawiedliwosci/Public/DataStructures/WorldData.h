#include "WorldData.generated.h"

#pragma once

USTRUCT(BlueprintType)
struct FBuildingData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAccessible;
};

USTRUCT(BlueprintType)
struct FInteractibleObjData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInteracted;
};

USTRUCT(BlueprintType)
struct FCutsceneTriggerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAlreadySeen;
};

USTRUCT(BlueprintType)
struct FFightEncounterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RespawnsLeft;
};

USTRUCT(BlueprintType)
struct FNPCData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpawnPointName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAlive;
};