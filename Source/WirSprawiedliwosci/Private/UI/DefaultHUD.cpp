// Po kim ty taki chitry na prawa autorskie?!

#include "UI/DefaultHUD.h"
#include "UI/PartyOverlay.h"
#include "UI/CombatOverlay.h"
#include "UI/InfoTextWidget.h"
#include "UI/VictoryDialogue.h"
#include "UI/InventorySlot.h"
#include "UI/InventoryWidget.h"
#include "UI/CharacterMenu.h"
#include "UI/AttributesMenu.h"
#include "UI/PartyMenu.h"
#include "UI/InteractPrompt.h"
#include "UI/CutscenePlayer.h"
#include "Components/Image.h"
//#include "Components/WidgetSwitcher.h"
#include "Items/Item.h"
#include "Kismet/GameplayStatics.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"

void ADefaultHUD::BeginPlay()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller == nullptr) return;
	if(PartyOverlayClass)
	{
		PartyOverlay = CreateWidget<UPartyOverlay>(Controller, PartyOverlayClass, FName("PartyOverlay"));
		PartyOverlay->InitializePartyIcons();
		if(UGameplayStatics::DoesSaveGameExist(TEXT("Save00"), 0)) 
		{
			PartyOverlay->AddToViewport();
		}
	}
	if (InfoTextWidgetClass)
	{
		InfoTextWidget = CreateWidget<UInfoTextWidget>(Controller, InfoTextWidgetClass, FName("InfoText"));
		InfoTextWidget->DefaultHUD = this;
		InfoTextWidget->AddToViewport();
	}
	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(Controller, InventoryWidgetClass, FName("Inventory"));
		InventoryWidget->SetInventorySlotsArray();
	}
	if (CharacterMenuClass)
	{
		CharacterMenu = CreateWidget<UCharacterMenu>(Controller, CharacterMenuClass, FName("CharacterMenu"));
		CharacterMenu->SetCharacterIcons();
		if (CharacterMenu->PartyMenu)
		{
			CharacterMenu->PartyMenu->UpdatePartyIcons();
		}
		//CharacterMenu->AttributesMenu = Cast<UAttributesMenu>(CharacterMenu->Switcher->GetChildAt(1));
	}
	if (InteractPromptClass)
	{
		InteractPrompt = CreateWidget<UInteractPrompt>(Controller, InteractPromptClass, FName("InteractPrompt"));
	}
}

void ADefaultHUD::SwitchToCombatOverlay()
{
	if (PartyOverlay)
	{
		PartyOverlay->RemoveFromViewport();
	}
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller && CombatOverlayClass)
	{
		CombatOverlay = CreateWidget<UCombatOverlay>(Controller, CombatOverlayClass, FName("CombatOverlay"));
		CombatOverlay->AddToViewport();
	}
}

void ADefaultHUD::SwitchToPartyOverlay()
{
	if (CombatOverlay)
	{
		CombatOverlay->RemoveFromViewport();
	}
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller && PartyOverlayClass)
	{
		PartyOverlay = CreateWidget<UPartyOverlay>(Controller, PartyOverlayClass, FName("PartyOverlay"));
		PartyOverlay->InitializePartyIcons();
		PartyOverlay->AddToViewport();
	}
}

void ADefaultHUD::ShowPartyOverlay()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::FromHex("#FF5F1F"), __FUNCTION__);
	}
	if (PartyOverlay)
	{
		PartyOverlay->AddToViewport();
		PartyOverlay->InitializePartyIcons();
	}
}

void ADefaultHUD::HidePartyOverlay()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::FromHex("#FF5F1F"), __FUNCTION__);
	}
	if (PartyOverlay)
	{
		PartyOverlay->RemoveFromViewport();
	}
}

void ADefaultHUD::ShowInfoText(FString Text)
{
	if (InfoTextWidget)
	{
		InfoTextWidget->ShowInfoText(Text);
	}
}

void ADefaultHUD::ShowGameOverDialogue()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller && GameOverDialogueClass)
	{
		GameOverDialogue = CreateWidget<UUserWidget>(Controller, GameOverDialogueClass, FName("GameOverDialogue"));
		GameOverDialogue->AddToViewport();
	}
}

void ADefaultHUD::ShowVictoryDialogue(int32 GainedExp, UItem* ItemReward, float MoneyEarned)
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller && VictoryDialogueClass)
	{
		VictoryDialogue = CreateWidget<UVictoryDialogue>(Controller, VictoryDialogueClass, FName("VictoryDialogue"));
		VictoryDialogue->SetExpRewardAmount(GainedExp);
		if (ItemReward)
		{
			VictoryDialogue->ItemReward->SetVisibility(ESlateVisibility::Visible);
			VictoryDialogue->ItemReward->SetNewItem(ItemReward);
		}
		VictoryDialogue->SetMoneyRewardAmount(MoneyEarned);
		VictoryDialogue->AddToViewport();
	}
}

void ADefaultHUD::ShowCharacterMenu()
{
	if (CharacterMenu)
	{
		//UGameplayStatics::GetPlayerController(this, 0)->SetPause(true);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		CharacterMenu->AddToViewport();
		CharacterMenu->SetCurrentAlly(Cast<AAlly>(GetWorld()->GetFirstPlayerController()->GetCharacter()));
		if(CharacterMenu->PartyMenu)
		{
			CharacterMenu->PartyMenu->UpdatePartyIcons();
		}
	}
}

void ADefaultHUD::ShowInteractPrompt(FString PromptText)
{
	if (InteractPrompt)
	{
		InteractPrompt->SetPromptText(PromptText);
		InteractPrompt->AddToViewport();
	}
}

void ADefaultHUD::HideInteractPrompt()
{
	if (InteractPrompt)
	{
		InteractPrompt->RemoveFromViewport();
	}
}

void ADefaultHUD::ShowInventory()
{
	if (InventoryWidget)
	{
		UGameplayStatics::GetPlayerController(this, 0)->SetPause(true);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		InventoryWidget->UpdateMoney();
		InventoryWidget->UpdateInventorySlots();
		InventoryWidget->AddToViewport();
	}
}

void ADefaultHUD::HideInventory()
{
	if (InventoryWidget)
	{
		UGameplayStatics::GetPlayerController(this, 0)->SetPause(false);
		ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(this));
		if(CurrentGameMode && CurrentGameMode->CurrentMode!=ECurrentMode::ECM_Combat)
		{
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		}
		InventoryWidget->RemoveFromViewport();
	}
}

void ADefaultHUD::ShowCutscenePlayer()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller == nullptr) return;
	Controller->DisableInput(Controller);
	//Controller->SetPause(true);
	if (CutscenePlayer)
	{
		CutscenePlayer->AddToViewport();
	}
	else
	{
		if (CutscenePlayerClass == nullptr) return;
		CutscenePlayer = CreateWidget<UCutscenePlayer>(Controller, CutscenePlayerClass, FName("CutscenePlayer"));
		CutscenePlayer->AddToViewport();
	}
}

void ADefaultHUD::HideCutscenePlayer()
{
	if (CutscenePlayer)
	{
		CutscenePlayer->RemoveFromViewport();
	}
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller == nullptr) return;
	//Controller->SetPause(false);
	Controller->EnableInput(Controller);
}

void ADefaultHUD::SetCutscenePlayerBrush(UMaterialInterface* Material)
{
	if (CutscenePlayer && CutscenePlayer->ImgVideo && Material)
	{
		CutscenePlayer->ImgVideo->SetBrushFromMaterial(Material);
	}
}

void ADefaultHUD::UpdateOverlays()
{
	if (PartyOverlay)
	{
		PartyOverlay->InitializePartyIcons();
	}
	if (CharacterMenu)
	{
		CharacterMenu->PartyMenu->UpdatePartyIcons();
		CharacterMenu->SetCharacterIcons();
	}
}
