// Po kim ty taki chitry na prawa autorskie?!

#include "UI/ShopInventoryWidget.h"
#include "UI/ShopInventorySlot.h"
#include "UI/DefaultHUD.h"
#include "Components/TextBlock.h"
#include "Items/Item.h"
#include "Characters/Shopkeeper.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "Kismet/GameplayStatics.h"

void UShopInventoryWidget::UpdateInventorySlots()
{
	if (Shopkeeper == nullptr || Shopkeeper->ShopInventory.IsEmpty()) return;
	UpdateMoney();
	for (int i = 0; i < InventorySlots.Num(); i++)
	{
		InventorySlots[i]->SetNewItem(nullptr);
		InventorySlots[i]->Quantity->SetText(FText::GetEmpty());
	}
	int i = 0;
	for (TPair<TSubclassOf<UItem>, float> Pair : Shopkeeper->ShopInventory)
	{
		UItem* Item = NewObject<UItem>(this, Pair.Key);
		Item->Quantity = 1;
		if (i == InventorySlots.Num()) return;
		UShopInventorySlot* ShopInventorySlot = Cast<UShopInventorySlot>(InventorySlots[i]);
		if (ShopInventorySlot)
		{
			ShopInventorySlot->SetNewItemWithPrice(Item, Pair.Value);
			ShopInventorySlot->OnItemClicked.AddDynamic(this, &UShopInventoryWidget::HandleShopItemClicked);
		}
		i++;
	}
}

void UShopInventoryWidget::HandleShopItemClicked(UItem* ClickedItem)
{
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode == nullptr || Shopkeeper == nullptr || ClickedItem == nullptr) return;
	float Price = *Shopkeeper->ShopInventory.Find(ClickedItem->GetClass());
	if (CurrentGameMode->Money < Price)
	{
		PlayErrorSound();
		AHUD* HUD = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD();
		if (HUD)
		{
			ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(HUD);
			if (CurrentHUD)
			{
				CurrentHUD->ShowInfoText(TEXT("Brak \u015brodk\u00f3w!"));
			}
		}
	}
	else
	{
		PlayPurchaseSound();
		CurrentGameMode->Money -= Price;
		ClickedItem->AddToInventory();
	}
	UpdateMoney();
}

void UShopInventoryWidget::PlayPurchaseSound()
{
	if (PurchaseSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PurchaseSound);
	}
}

void UShopInventoryWidget::PlayErrorSound()
{
	if (ErrorSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ErrorSound);
	}
}
