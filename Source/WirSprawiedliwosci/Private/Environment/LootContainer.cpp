#include "Environment/LootContainer.h"
#include "Items/Item.h"
#include "UI/ItemDialogue.h"

void ALootContainer::BeginPlay()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		ItemDialogue = CreateWidget<UItemDialogue>(Controller, ItemDialogueClass);
		ItemDialogue->OnItemTaken.AddDynamic(this, &ALootContainer::HandleItemTaken);
	}
}

void ALootContainer::ChooseRandomItem()
{
	if (ItemClasses.IsEmpty()) return;
	int32 Index = FMath::RandRange(0, ItemClasses.Num() - 1);
	Item = NewObject<UItem>(GetOuter(), ItemClasses[Index]);
	Item->Quantity = ItemQuantity;
}

void ALootContainer::Interact_Implementation()
{
	Super::Interact_Implementation();
	if (!bInteracted)
	{
		ChooseRandomItem();
		APlayerController* Controller = GetWorld()->GetFirstPlayerController();
		if(Controller)
		{
			ItemDialogue->SetNewItem(Item);
			Controller->bShowMouseCursor = true;
			ItemDialogue->AddToViewport();
		}
	}
}

void ALootContainer::HandleItemTaken()
{
	UE_LOG(LogTemp, Warning, TEXT("Marking trashcan as interacted!"));
	bInteracted = true;
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	ItemDialogue->RemoveFromViewport();
}
