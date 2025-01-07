// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/InteractibleObject.h"
#include "LootContainer.generated.h"

class UItem;
class UItemDialogue;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API ALootContainer : public AInteractibleObject
{
	GENERATED_BODY()
public:
	void ChooseRandomItem();
	virtual void Interact_Implementation() override;

	UFUNCTION()
	void HandleItemTaken();

	UItem* Item;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UItem>> ItemClasses;

	UPROPERTY(EditAnywhere)
	int32 ItemQuantity;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemDialogue> ItemDialogueClass;

	UPROPERTY()
	UItemDialogue* ItemDialogue;

protected:
	void BeginPlay() override;
};
