#include "DataStructures/CharacterStatData.h"
#include "DataStructures/SkillData.h"
#include "DataStructures/EffectData.h"
#include "AllyData.generated.h"

#pragma once

USTRUCT(BlueprintType)
struct FAllyData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterStatData StatData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkillData> Skills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEffectData> AppliedEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
};