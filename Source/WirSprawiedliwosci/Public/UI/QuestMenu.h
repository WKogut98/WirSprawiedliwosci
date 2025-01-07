// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestMenu.generated.h"

class UListView;
class UTextBlock;
class UTreeView;
class UQuestListEntry;
class UQuestObjective;
class UObjectiveCollection;
class AQuest;
class UUserWidget;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API UQuestMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void HandleQuestListEntryClicked(UObjectiveCollection* ChosenQuestRoot);

	UFUNCTION(BlueprintCallable)
	void InitializeQuestList();

	UFUNCTION(BlueprintCallable)
	void AddQuestListEntryEventCallback(UUserWidget* EntryWidget);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UObjectiveCollection* QuestRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UListView* QuestListView;

	UPROPERTY(meta = (BindWidget))
	UTreeView* ObjectiveTree;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuestListEntry> QuestListEntryClass;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuestDescription;
};
