#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDialogue.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemTakenDelegate);

class UInventorySlot;
class UButton;
class UItem;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UItemDialogue : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetNewItem(UItem* NewItem);

	UFUNCTION(BlueprintCallable)
	void HandleTakeItem();

	UPROPERTY()
	UItem* Item;

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* ItemSlot;

	UPROPERTY(meta = (BindWidget))
	UButton* TakeItemButton;

	FOnItemTakenDelegate OnItemTaken;
};
