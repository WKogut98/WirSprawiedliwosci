// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuestMenu.h"
#include "UI/QuestListEntry.h"
//#include "UI/ObjectiveEntry.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Components/TreeView.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "GameMechanics/Quests/Quest.h"
#include "GameMechanics/Quests/ObjectiveCollection.h"
#include "Kismet/GameplayStatics.h"

void UQuestMenu::HandleQuestListEntryClicked(UObjectiveCollection* ChosenQuestRoot)
{
	ObjectiveTree->ClearListItems();
	QuestRoot = ChosenQuestRoot;
	if (QuestRoot)
	{
		ObjectiveTree->AddItem(QuestRoot);
		ObjectiveTree->ExpandAll();
		AQuest* Quest = Cast<AQuest>(QuestRoot->GetOuter());
		if (Quest)
		{
			QuestDescription->SetText(Quest->Description);
		}
	}
}

void UQuestMenu::InitializeQuestList()
{
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(
		UGameplayStatics::GetGameMode(GetWorld())
	);
	QuestListView->ClearListItems();
	if (CurrentGameMode == nullptr) return;
	if (CurrentGameMode->Quests.IsEmpty()) return;
	for (AQuest* Quest : CurrentGameMode->Quests)
	{
		if (Quest == nullptr) continue;
		QuestListView->AddItem(Quest->GetRootObjectiveCollection());
	}
}

void UQuestMenu::AddQuestListEntryEventCallback(UUserWidget* EntryWidget)
{
	UQuestListEntry* QuestListEntry = Cast<UQuestListEntry>(EntryWidget);
	if (QuestListEntry)
	{
		QuestListEntry->OnQuestListEntryClicked.AddDynamic(this, &UQuestMenu::HandleQuestListEntryClicked);
	}
}
