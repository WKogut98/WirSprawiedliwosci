// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartyOverlay.generated.h"

class UCharacterIcon;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UPartyOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void InitializePartyIcons();

	TArray<UCharacterIcon*> GetIconsAsArray();

	UPROPERTY(meta=(BindWidget))
	UCharacterIcon* PartyIcon1;

	UPROPERTY(meta = (BindWidget))
	UCharacterIcon* PartyIcon2;

	UPROPERTY(meta = (BindWidget))
	UCharacterIcon* PartyIcon3;

	UPROPERTY(meta = (BindWidget))
	UCharacterIcon* PartyIcon4;
};
