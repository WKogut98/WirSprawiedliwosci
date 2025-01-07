// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InfoTextWidget.generated.h"

class UTextBlock;
class ADefaultHUD;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UInfoTextWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void ShowInfoText(FString Text);
	void ClearInfoText();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoText;

	UPROPERTY()
	ADefaultHUD* DefaultHUD;

	FTimerHandle ShowTextTimer;
};
