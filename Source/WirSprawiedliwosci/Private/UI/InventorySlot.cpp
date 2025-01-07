#include "UI/InventorySlot.h"
#include "UI/ItemDetails.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/Item.h"

void UInventorySlot::SetNewItem(UItem* NewItem)
{
	if (NewItem)
	{
		Item = NewItem;
		ItemImage->SetBrushFromTexture(Item->ItemImage);
		if(Item->Quantity > 1)
		{
			Quantity->SetText(FText::FromString(FString::FromInt(Item->Quantity)));
		}
		else
		{
			Quantity->SetText(FText::FromString(TEXT("")));
		}
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Item = nullptr;
		ItemImage->SetBrushFromTexture(nullptr);
		Quantity->SetText(FText::FromString(TEXT("")));
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UInventorySlot::HandleOnHover()
{
	if (Item == nullptr) return;
	if (ItemDetails)
	{
		ShowItemDetails();
	}
}

void UInventorySlot::HandleOnEndHover()
{
	if (Item == nullptr) return;
	if (ItemDetails)
	{
		HideItemDetails();
	}
}

void UInventorySlot::HandleClicked()
{
	if (ItemDetails)
	{
		HideItemDetails();
	}
	if(Item)
	{
		OnItemClicked.Broadcast(Item);
	}
}

void UInventorySlot::CreateItemDetails()
{
	if (Item == nullptr || ItemDetailsClass == nullptr) return;
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	ItemDetails = CreateWidget<UItemDetails>(Controller, ItemDetailsClass);
	ItemDetails->SetNewItem(Item);
}

void UInventorySlot::ShowItemDetails()
{
	if (Item == nullptr) return;
	if (ItemDetails == nullptr)
	{
		CreateItemDetails();
	}
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller == nullptr) return;
	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	double MouseX;
	double MouseY;
	Controller->GetMousePosition(MouseX, MouseY);
	FMath::Clamp(MouseX, 0.f, ViewportSize.X);
	FMath::Clamp(MouseY, 0.f, ViewportSize.Y);
	ItemDetails->AddToViewport();
	ItemDetails->SetPositionInViewport(FVector2D(MouseX, MouseY));
}

void UInventorySlot::HideItemDetails()
{
	if (!ItemDetails->IsValidLowLevel() || !ItemDetails->IsInViewport()) return;
	ItemDetails->RemoveFromViewport();
}
