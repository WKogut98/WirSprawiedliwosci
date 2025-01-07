// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemClickedDelegate, UItem*, ClickedItem);

class UImage;
class UTextBlock;
class UItem;
class UItemDetails;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetNewItem(UItem* NewItem);

	//mouse event callbacks
	UFUNCTION(BlueprintCallable)
	void HandleOnHover();

	UFUNCTION(BlueprintCallable)
	void HandleOnEndHover();

	UFUNCTION(BlueprintCallable)
	void HandleClicked();

	//item detail box
	UFUNCTION(BlueprintCallable)
	void CreateItemDetails();

	void ShowItemDetails();
	void HideItemDetails();

	UPROPERTY()
	UItem* Item;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Quantity;

	UPROPERTY(EditAnywhere, Category = Tooltip)
	TSubclassOf<UItemDetails> ItemDetailsClass;

	UPROPERTY()
	UItemDetails* ItemDetails;

	UPROPERTY()
	FOnItemClickedDelegate OnItemClicked;
};
