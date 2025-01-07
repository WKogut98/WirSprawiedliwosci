#include "QuestData.generated.h"

#pragma once

USTRUCT(BlueprintType)
struct FQuestData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ActiveObjectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> CompletedObjectives;
};