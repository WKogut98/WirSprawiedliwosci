#include "Engine/DataTable.h"
#include "CharacterStatData.generated.h"

#pragma once

USTRUCT(BlueprintType)
struct FCharacterStatData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ACombatCharacter> CharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Vitality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Magic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 CapacityForGenocide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 HealingProficiency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Defence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Dodge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Luck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Accuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 StunResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 OppressionResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 StinkResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 DisgraceResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 DeathwishResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 TiredResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 DrunkResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Experience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExpToNextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttributePoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Rank;
};