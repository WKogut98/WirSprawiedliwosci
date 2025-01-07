// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DefaultHUD.generated.h"

class UUserWidget;
class UPartyOverlay;
class UCombatOverlay;
class UInfoTextWidget;
class UVictoryDialogue;
class UInventoryWidget;
class UCharacterMenu;
class UInteractPrompt;
class UCutscenePlayer;
class UItem;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API ADefaultHUD : public AHUD
{
	GENERATED_BODY()
public:
	void SwitchToCombatOverlay();

	UFUNCTION(BlueprintCallable)
	void SwitchToPartyOverlay();

	void ShowPartyOverlay();
	void HidePartyOverlay();

	UFUNCTION(BlueprintCallable)
	void ShowInfoText(FString Text);

	void ShowGameOverDialogue();
	void ShowVictoryDialogue(int32 GainedExp, UItem* ItemReward, float MoneyEarned);
	void ShowCharacterMenu();

	UFUNCTION(BlueprintCallable)
	void ShowInteractPrompt(FString PromptText);

	UFUNCTION(BlueprintCallable)
	void HideInteractPrompt();

	UFUNCTION(BlueprintCallable)
	void ShowInventory();

	UFUNCTION(BlueprintCallable)
	void HideInventory();

	UFUNCTION(BlueprintCallable)
	void ShowCutscenePlayer();

	UFUNCTION(BlueprintCallable)
	void HideCutscenePlayer();

	void SetCutscenePlayerBrush(UMaterialInterface* Material);

	void UpdateOverlays();

	UPROPERTY(BlueprintReadOnly)
	UPartyOverlay* PartyOverlay;

	UPROPERTY()
	UCombatOverlay* CombatOverlay;

	UPROPERTY()
	UInfoTextWidget* InfoTextWidget;

	UPROPERTY(BlueprintReadOnly)
	UInventoryWidget* InventoryWidget;

	UPROPERTY(BlueprintReadOnly)
	UCharacterMenu* CharacterMenu;

	UPROPERTY()
	UUserWidget* GameOverDialogue;

	UPROPERTY()
	UVictoryDialogue* VictoryDialogue;

	UPROPERTY()
	UInteractPrompt* InteractPrompt;

	UPROPERTY()
	UCutscenePlayer* CutscenePlayer;

	//getters and setters
	FORCEINLINE UPartyOverlay* GetPartyOverlay() const { return PartyOverlay; };
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPartyOverlay> PartyOverlayClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCombatOverlay> CombatOverlayClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInfoTextWidget> InfoTextWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> GameOverDialogueClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UVictoryDialogue> VictoryDialogueClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterMenu> CharacterMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInteractPrompt> InteractPromptClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCutscenePlayer> CutscenePlayerClass;
};
