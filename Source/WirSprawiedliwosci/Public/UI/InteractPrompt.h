// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractPrompt.generated.h"

class UTextBlock;
class UImage;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UInteractPrompt : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetPromptText(FString NewText);

	UPROPERTY(meta = (BindWidget))
	UImage* KeyIcon;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PromptText;
};
