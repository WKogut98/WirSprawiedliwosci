// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestObjective.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompletedDelegate, UQuestObjective*, CompletedObjective);

UCLASS(BlueprintType, Blueprintable)
class WIRSPRAWIEDLIWOSCI_API UQuestObjective : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActivateObjective(bool bShowOnScreen);

	virtual void ActivateObjective_Implementation(bool bShowOnScreen);

	UFUNCTION(BlueprintCallable)
	void Complete();

	void AwardExperience();

	void PlayCompletedSound();

	void PlayActivateSound();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
	FName ObjectiveName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive;

	UPROPERTY(BlueprintReadWrite)
	bool bCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
	int32 Experience;

	UPROPERTY(EditAnywhere)
	FString ActivateText;

	UPROPERTY(EditAnywhere)
	FString CompletedText;

	UPROPERTY(EditAnywhere)
	USoundBase* CompletedSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ActivateSound;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnObjectiveCompletedDelegate OnObjectiveCompleted;
};
