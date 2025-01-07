#include "GameMechanics/Effect.h"
#include "EffectData.generated.h"

#pragma once

USTRUCT(BlueprintType)
struct FEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UEffect> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Duration;
};