// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMechanics/Quests/QuestObjective.h"
#include "ObjectiveCollection.generated.h"

class UQuestObjective;
UCLASS(BlueprintType, Blueprintable)
class WIRSPRAWIEDLIWOSCI_API UObjectiveCollection : public UQuestObjective
{
	GENERATED_BODY()
public:
	virtual void ActivateObjective_Implementation(bool bShowOnScreen) override;

	UFUNCTION(BlueprintCallable)
	void ActivateNextObjective();

	void ActivateAllObjectives();
	UQuestObjective* FindNextIncompleteObjective();

	UFUNCTION()
	void HandleObjectiveCompleted(UQuestObjective* CompletedObjective);

	UFUNCTION(BlueprintCallable)
	void AddObjective(UQuestObjective* Objective);

	UFUNCTION(BlueprintCallable)
	UQuestObjective* FindObjectiveByName(FName Name);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
	bool bOrderRequired;

	UPROPERTY()
	TArray<UQuestObjective*> Objectives;

	FTimerHandle ObjectiveTimer;
};
