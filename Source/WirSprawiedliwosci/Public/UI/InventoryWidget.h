#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UTextBlock;
class UInventorySlot;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetInventorySlotsArray();
	virtual void UpdateInventorySlots();
	void UpdateMoney();

	UPROPERTY()
	TArray<UInventorySlot*> InventorySlots;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoneyAmount;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot1;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot2;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot3;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot4;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot5;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot6;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot7;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot8;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot9;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot10;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot11;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot12;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot13;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot14;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* InventorySlot15;
};
