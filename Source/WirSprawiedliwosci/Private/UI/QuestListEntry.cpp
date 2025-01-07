// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuestListEntry.h"
#include "Styling/SlateColor.h"
#include "Components/TextBlock.h"
#include "GameMechanics/Quests/Quest.h"
#include "GameMechanics/Quests/ObjectiveCollection.h"

void UQuestListEntry::HandleListObjectSet(UObject* Object)
{
	QuestRoot = Cast<UObjectiveCollection>(Object);
	if (QuestRoot)
	{
		QuestName->SetText(FText::FromName(QuestRoot->ObjectiveName));
	}
	else
	{
		QuestName->SetText(FText::FromString(TEXT("")));
	}
}

void UQuestListEntry::ChangeTextColor(FSlateColor Color)
{
	QuestName->SetColorAndOpacity(Color);
}

void UQuestListEntry::HandleClicked()
{
	OnQuestListEntryClicked.Broadcast(QuestRoot);
}
