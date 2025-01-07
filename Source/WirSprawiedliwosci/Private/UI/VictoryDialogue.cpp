// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VictoryDialogue.h"
#include"Components/TextBlock.h"
#include "Components/Image.h"

void UVictoryDialogue::SetExpRewardAmount(int32 NewExpReward)
{
	if (NewExpReward == 0)
	{
		ExpReward->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		FString NewText = FString::FromInt(NewExpReward) + TEXT(" PD");
		ExpReward->SetText(FText::FromString(NewText));
	}
}

void UVictoryDialogue::SetMoneyRewardAmount(float NewMoneyReward)
{
	if (NewMoneyReward == 0.f)
	{
		MoneyIcon->SetVisibility(ESlateVisibility::Collapsed);
		MoneyReward->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		FString Amount = FString::Printf(TEXT("%.2f PLN"), NewMoneyReward);
		MoneyReward->SetText(FText::FromString(Amount));
	}
}
