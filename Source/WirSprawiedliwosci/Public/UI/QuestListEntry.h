// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "QuestListEntry.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestListEntryClickedDelegate, UObjectiveCollection*, ChosenQuestRoot);

class UTextBlock;
class UObjectiveCollection;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API UQuestListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void HandleListObjectSet(UObject* Object);

	UFUNCTION(BlueprintCallable)
	void ChangeTextColor(FSlateColor Color);

	UFUNCTION(BlueprintCallable)
	void HandleClicked();
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSlateColor HoveredTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSlateColor CompletedTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSlateColor RegularTextColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UObjectiveCollection* QuestRoot;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnQuestListEntryClickedDelegate OnQuestListEntryClicked;
};
