// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ObjectiveEntry.h"
#include "Components/TextBlock.h"
#include "Components/TreeView.h"
#include "GameMechanics/Quests/QuestObjective.h"
#include "GameMechanics/Quests/ObjectiveCollection.h"

void UObjectiveEntry::HandleListObjectSet(UObject* Object)
{
	UQuestObjective* CurrentObjective = Cast<UQuestObjective>(Object);
	if (CurrentObjective)
	{
		ObjectiveName->SetText(FText::FromName(CurrentObjective->ObjectiveName));
		UObjectiveCollection* ObjCollection = Cast<UObjectiveCollection>(CurrentObjective);
		if(ObjCollection)
		{
			for (UQuestObjective* Objective : ObjCollection->Objectives)
			{
				ObjectivesTree->AddItem(Objective);
			}
			ObjectivesTree->ExpandAll();
		}
	}
	else
	{
		ObjectiveName->SetText(FText::FromString(TEXT("")));
	}
}
