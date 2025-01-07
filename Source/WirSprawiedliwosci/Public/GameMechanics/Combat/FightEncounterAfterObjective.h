// Po kim ty taki chitry na prawa autorskie?!

#pragma once

#include "CoreMinimal.h"
#include "GameMechanics/Combat/FightEncounter.h"
#include "FightEncounterAfterObjective.generated.h"

/**
 * 
 */
class AQuest;
class UQuestObjective;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API AFightEncounterAfterObjective : public AFightEncounter
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void HandleObjectiveCompleted(UQuestObjective* CompletedObjective);

	UPROPERTY(EditAnywhere)
	AQuest* RequiredQuest;

	UPROPERTY(EditAnywhere)
	FName RequiredObjectiveName;

	UPROPERTY()
	UQuestObjective* Objective;

protected:
	virtual void BeginPlay() override;
};
