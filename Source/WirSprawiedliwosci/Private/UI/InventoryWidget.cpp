#include "UI/InventoryWidget.h"
#include "UI/InventorySlot.h"
#include "Components/TextBlock.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidget::SetInventorySlotsArray()
{
	if(InventorySlots.IsEmpty())
	{
		InventorySlots.Add(InventorySlot1);
		InventorySlots.Add(InventorySlot2);
		InventorySlots.Add(InventorySlot3);
		InventorySlots.Add(InventorySlot4);
		InventorySlots.Add(InventorySlot5);
		InventorySlots.Add(InventorySlot6);
		InventorySlots.Add(InventorySlot7);
		InventorySlots.Add(InventorySlot8);
		InventorySlots.Add(InventorySlot9);
		InventorySlots.Add(InventorySlot10);
		InventorySlots.Add(InventorySlot11);
		InventorySlots.Add(InventorySlot12);
		InventorySlots.Add(InventorySlot13);
		InventorySlots.Add(InventorySlot14);
		InventorySlots.Add(InventorySlot15);
	}
}

void UInventoryWidget::UpdateInventorySlots()
{
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode == nullptr || InventorySlots.IsEmpty()) return;
	for (int i = 0; i < InventorySlots.Num(); i++)
	{
		if (i < CurrentGameMode->Inventory.Num())
		{
			InventorySlots[i]->SetNewItem(CurrentGameMode->Inventory[i]);
		}
		else
		{
			InventorySlots[i]->SetNewItem(nullptr);
		}
	}
}

void UInventoryWidget::UpdateMoney()
{
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode == nullptr || MoneyAmount == nullptr) return;
	FString Amount = FString::Printf(TEXT("%.2f PLN"), CurrentGameMode->Money);
	MoneyAmount->SetText(FText::FromString(Amount));
}
