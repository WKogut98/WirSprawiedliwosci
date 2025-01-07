// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterWidgetComponent.h"
#include "UI/CharacterWidget.h"
#include "UI/EffectIcon.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCharacterWidgetComponent::SetHealthPercent(float Percent)
{
	if (CharacterWidget == nullptr)
	{
		CharacterWidget = Cast<UCharacterWidget>(GetUserWidgetObject());
	}
	//CharacterWidget->HealthBar = Cast<UProgressBar>(CharacterWidget->GetWidgetFromName(FName("HealthBar")));
	if(CharacterWidget && CharacterWidget->HealthBar)
	{
		//CharacterWidget->HealthBar->SetVisibility(ESlateVisibility::Hidden);
		CharacterWidget->HealthBar->SetPercent(Percent);
		//CharacterWidget->HealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCharacterWidgetComponent::SetManaPercent(float Percent)
{
	if (CharacterWidget == nullptr)
	{
		CharacterWidget = Cast<UCharacterWidget>(GetUserWidgetObject());
	}
	if(CharacterWidget)
	{
		CharacterWidget->ManaBar->SetPercent(Percent);
		CharacterWidget->ManaBar->SynchronizeProperties();
	}
}

void UCharacterWidgetComponent::SetRagePercent(float Percent)
{
	if (CharacterWidget == nullptr)
	{
		CharacterWidget = Cast<UCharacterWidget>(GetUserWidgetObject());
	}
	if (CharacterWidget)
	{
		CharacterWidget->RageBar->SetPercent(Percent);
	}
}

void UCharacterWidgetComponent::UpdateEffectIcons(TArray<UEffect*> Effects)
{
	TArray<UEffectIcon*> EffectIcons = GetEffectIconsAsArray();
	int32 i = 0;
	for (UEffectIcon* EffectIcon : EffectIcons)
	{
		EffectIcon->ClearIcon();
		if (i < Effects.Num() && Effects[i])
		{
			EffectIcon->SetNewEffect(Effects[i]);
			i++;
		}
	}
}

TArray<UEffectIcon*> UCharacterWidgetComponent::GetEffectIconsAsArray()
{
	TArray<UEffectIcon*> EffectIcons;
	if (CharacterWidget == nullptr) return EffectIcons;
	if (CharacterWidget->EffectIcon1)
	{
		EffectIcons.Add(CharacterWidget->EffectIcon1);
	}
	if (CharacterWidget->EffectIcon2)
	{
		EffectIcons.Add(CharacterWidget->EffectIcon2);
	}
	if (CharacterWidget->EffectIcon3)
	{
		EffectIcons.Add(CharacterWidget->EffectIcon3);
	}
	if (CharacterWidget->EffectIcon4)
	{
		EffectIcons.Add(CharacterWidget->EffectIcon4);
	}
	if (CharacterWidget->EffectIcon5)
	{
		EffectIcons.Add(CharacterWidget->EffectIcon5);
	}
	return EffectIcons;
}

void UCharacterWidgetComponent::ShowRageBar()
{
	if (CharacterWidget == nullptr)
	{
		CharacterWidget = Cast<UCharacterWidget>(GetUserWidgetObject());
	}
	if(CharacterWidget)
	{
		CharacterWidget->ShowRageBar();
	}
}

void UCharacterWidgetComponent::SetSpecialInfoText(FString DisplayedText)
{
	if (CharacterWidget && CharacterWidget->SpecialInfoText)
	{
		CharacterWidget->SpecialInfoText->SetText(FText::FromString(DisplayedText));
	}
}

void UCharacterWidgetComponent::ShowSpecialInfoText()
{
	if (CharacterWidget && CharacterWidget->SpecialInfoText)
	{
		CharacterWidget->SpecialInfoText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCharacterWidgetComponent::HideSpecialInfoText()
{
	if (CharacterWidget && CharacterWidget->SpecialInfoText)
	{
		CharacterWidget->SpecialInfoText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCharacterWidgetComponent::SetArrowToWhite()
{
	if (CharacterWidget)
	{
		CharacterWidget->SetArrowToWhite();
	}
}

void UCharacterWidgetComponent::SetArrowToColor(FColor Color)
{
	if (CharacterWidget)
	{
		CharacterWidget->SetArrowToColor(Color);
	}
}

void UCharacterWidgetComponent::HideRageBar()
{
	if (CharacterWidget == nullptr)
	{
		CharacterWidget = Cast<UCharacterWidget>(GetUserWidgetObject());
	}
	if(CharacterWidget)
	{
		CharacterWidget->HideRageBar();
	}
}

void UCharacterWidgetComponent::ShowArrow()
{
	if (CharacterWidget)
	{
		CharacterWidget->ShowArrow();
	}
}

void UCharacterWidgetComponent::HideArrow()
{
	if (CharacterWidget)
	{
		CharacterWidget->HideArrow();
	}
}



