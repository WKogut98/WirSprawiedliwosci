#include "UI/ItemDialogue.h"
#include "UI/InventorySlot.h"
#include "Items/Item.h"

void UItemDialogue::SetNewItem(UItem* NewItem)
{
	Item = NewItem;
	ItemSlot->SetNewItem(Item);
}

void UItemDialogue::HandleTakeItem()
{
	if (Item)
	{
		Item->AddToInventory();
	}
	OnItemTaken.Broadcast();
}
