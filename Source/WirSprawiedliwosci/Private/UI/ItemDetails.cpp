#include "UI/ItemDetails.h"
#include "Components/TextBlock.h"
#include "Items/Item.h"

void UItemDetails::SetNewItem(UItem* NewItem)
{
	if (NewItem == nullptr)
	{
		ItemName->SetText(FText::FromString(TEXT("")));
		Description->SetText(FText::FromString(TEXT("")));
		Quote->SetText(FText::FromString(TEXT("")));
	}
	if (!NewItem->ItemName.IsEmpty())
	{
		ItemName->SetText(FText::FromString(NewItem->ItemName));
	}
	if (!NewItem->Description.IsEmpty())
	{
		Description->SetText(FText::FromString(NewItem->Description));
	}
	else
	{
		NewItem->BuildDescription();
		Description->SetText(FText::FromString(NewItem->Description));
	}
	if (!NewItem->Quote.IsEmpty())
	{
		Quote->SetText(FText::FromString(NewItem->Quote));
	}
}
