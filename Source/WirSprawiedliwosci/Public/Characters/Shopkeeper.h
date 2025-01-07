// Po kim ty taki chitry na prawa autorskie?!

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC.h"
#include "Shopkeeper.generated.h"

class UItem;
class UShopInventoryWidget;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API AShopkeeper : public ANPC
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<UItem>, float> ShopInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UShopInventoryWidget> InventoryWidgetClass;
};
