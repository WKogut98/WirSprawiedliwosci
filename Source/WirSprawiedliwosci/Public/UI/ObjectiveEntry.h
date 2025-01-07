// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ObjectiveEntry.generated.h"

class UTextBlock;
class UTreeView;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UObjectiveEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void HandleListObjectSet(UObject* Object);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ObjectiveName;

	UPROPERTY(meta = (BindWidget))
	UTreeView* ObjectivesTree;
};
