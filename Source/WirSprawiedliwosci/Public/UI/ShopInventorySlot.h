// Po kim ty taki chitry na prawa autorskie?!

#pragma once

#include "CoreMinimal.h"
#include "UI/InventorySlot.h"
#include "ShopInventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UShopInventorySlot : public UInventorySlot
{
	GENERATED_BODY()
public:
	void SetNewItemWithPrice(UItem* NewItem, float Price);
};
