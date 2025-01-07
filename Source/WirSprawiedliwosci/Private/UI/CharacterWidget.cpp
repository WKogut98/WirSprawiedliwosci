// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"


void UCharacterWidget::ShowRageBar()
{
	if (RageBar)
	{
		RageBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCharacterWidget::HideRageBar()
{
	if (RageBar)
	{
		RageBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCharacterWidget::ShowArrow()
{
	if (Arrow)
	{
		Arrow->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCharacterWidget::SetArrowToWhite()
{
	if (Arrow && ArrowWhite)
	{
		Arrow->SetBrushFromTexture(ArrowWhite, false);
	}
}

void UCharacterWidget::SetArrowToColor(FColor Color)
{
	if (!Arrow || !ArrowRed || !ArrowGreen) return;
	if (Color == FColor::Red)
	{
		Arrow->SetBrushFromTexture(ArrowRed, false);
	}
	if (Color == FColor::Green)
	{
		Arrow->SetBrushFromTexture(ArrowGreen, false);
	}
}

void UCharacterWidget::HideArrow()
{
	if(Arrow)
	{
		Arrow->SetVisibility(ESlateVisibility::Hidden);
	}
}
