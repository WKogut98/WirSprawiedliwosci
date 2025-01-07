// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMechanics/Quests/ObjectiveCollection.h"

void UObjectiveCollection::ActivateObjective_Implementation(bool bShowOnScreen)
{
	Super::ActivateObjective_Implementation(bShowOnScreen);
	if (Objectives.IsEmpty()) return;
	if (bOrderRequired) ActivateNextObjective();
	else ActivateAllObjectives();
}

void UObjectiveCollection::ActivateNextObjective()
{
	UQuestObjective* NextObjective = FindNextIncompleteObjective();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::FromHex("#FF5F1F"), __FUNCTION__);
	}
	if (NextObjective)
	{
		NextObjective->ActivateObjective(true);
		NextObjective->OnObjectiveCompleted.AddDynamic(this, &UObjectiveCollection::HandleObjectiveCompleted);
	}
	else
	{
		Complete();
		//GetWorld()->GetTimerManager().SetTimer(ObjectiveTimer, this, &UQuestObjective::Complete, 1.3f);
	}
}

void UObjectiveCollection::ActivateAllObjectives()
{
	
	for (UQuestObjective* Objective : Objectives)
	{
		Objective->ActivateObjective(true);
		Objective->OnObjectiveCompleted.AddDynamic(this, &UObjectiveCollection::HandleObjectiveCompleted);
	}
}

UQuestObjective* UObjectiveCollection::FindNextIncompleteObjective()
{
	for (UQuestObjective* Objective : Objectives)
	{
		if (!Objective->bCompleted) return Objective;
	}
	return nullptr;
}

void UObjectiveCollection::HandleObjectiveCompleted(UQuestObjective* CompletedObjective)
{
	CompletedObjective->OnObjectiveCompleted.Remove(this, FName("UObjectiveCollection::HandleObjectiveCompleted"));
	if (bOrderRequired)
	{
		ActivateNextObjective();
		return;
	}
	if (!bOrderRequired && FindNextIncompleteObjective() == nullptr)
	{
		Complete();
		return;
	}
	//GetWorld()->GetTimerManager().SetTimer(ObjectiveTimer, this, &UObjectiveCollection::ActivateNextObjective, 1.3f);
}

void UObjectiveCollection::AddObjective(UQuestObjective* Objective)
{
	Objectives.AddUnique(Objective);
}

UQuestObjective* UObjectiveCollection::FindObjectiveByName(FName Name)
{
	for (UQuestObjective* Objective : Objectives)
	{
		if (Objective->ObjectiveName.IsEqual(Name))
		{
			return Objective;
		}
	}
	return nullptr;
}
