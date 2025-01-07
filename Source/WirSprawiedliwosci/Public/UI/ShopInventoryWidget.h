// Po kim ty taki chitry na prawa autorskie?!

#pragma once

#include "CoreMinimal.h"
#include "UI/InventoryWidget.h"
#include "ShopInventoryWidget.generated.h"

class AShopkeeper;
class UItem;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UShopInventoryWidget : public UInventoryWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void UpdateInventorySlots() override;

	UFUNCTION()
	void HandleShopItemClicked(UItem* ClickedItem);

	void PlayPurchaseSound();
	void PlayErrorSound();

	UPROPERTY(BlueprintReadWrite)
	AShopkeeper* Shopkeeper;

	UPROPERTY(EditAnywhere)
	USoundBase* PurchaseSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ErrorSound;
};
