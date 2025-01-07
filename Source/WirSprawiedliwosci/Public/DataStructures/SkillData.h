#include "Engine/DataTable.h"
#include "SkillData.generated.h"

#pragma once

class USkill;
class UEffect;

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USkill> SkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAccuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseCritRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StunRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SelfDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SelfHeal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UEffect> InflictedEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InflictedEffectValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InflictedEffectDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UEffect> SelfEffect1Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SelfEffect1Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SelfEffect1Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UEffect> SelfEffect2Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SelfEffect2Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SelfEffect2Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEquipped;
};