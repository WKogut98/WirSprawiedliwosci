// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VictoryDialogue.generated.h"

class UTextBlock;
class UInventorySlot;
class UImage;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API UVictoryDialogue : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetExpRewardAmount(int32 NewExpReward);
	void SetMoneyRewardAmount(float NewMoneyReward);

	UPROPERTY(meta = (BindWidget))
	UInventorySlot* ItemReward;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ExpReward;

	UPROPERTY(meta = (BindWidget))
	UImage* MoneyIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoneyReward;
};
