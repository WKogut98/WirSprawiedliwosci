// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartyMenu.generated.h"

class UMenuCharacterIcon;
class AAlly;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UPartyMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdatePartyIcons();
	
	UFUNCTION()
	void SwapCharacter(ASpriteCharacter* ClickedCharacter);

	UFUNCTION(BlueprintCallable)
	void ActivateSwapping();

	TArray<UMenuCharacterIcon*> GetPartyIconsAsArray();

	UPROPERTY(meta = (BindWidget))
	UMenuCharacterIcon* PartyIcon1;

	UPROPERTY(meta = (BindWidget))
	UMenuCharacterIcon* PartyIcon2;

	UPROPERTY(meta = (BindWidget))
	UMenuCharacterIcon* PartyIcon3;

	UPROPERTY(meta = (BindWidget))
	UMenuCharacterIcon* PartyIcon4;

	UPROPERTY()
	bool bCanSwapCharacters = false;

	UPROPERTY()
	AAlly* CurrentAlly;
};
