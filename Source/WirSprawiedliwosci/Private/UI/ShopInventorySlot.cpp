// Po kim ty taki chitry na prawa autorskie?!


#include "UI/ShopInventorySlot.h"
#include "Components/TextBlock.h"

void UShopInventorySlot::SetNewItemWithPrice(UItem* NewItem, float Price)
{
	Super::SetNewItem(NewItem);
	FString Amount = FString::Printf(TEXT("%.2f z\u0142"), Price);
	Quantity->SetText(FText::FromString(Amount));
}
