// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InfoTextWidget.h"
#include "UI/DefaultHUD.h"
#include "Components/TextBlock.h"

void UInfoTextWidget::ShowInfoText(FString Text)
{
	InfoText->SetText(FText::FromString(Text));
	if (DefaultHUD)
	{
		DefaultHUD->GetWorldTimerManager().SetTimer(ShowTextTimer, this, &UInfoTextWidget::ClearInfoText, 1.5f);
	}
}

void UInfoTextWidget::ClearInfoText()
{
	InfoText->SetText(FText::FromString(TEXT("")));
}
