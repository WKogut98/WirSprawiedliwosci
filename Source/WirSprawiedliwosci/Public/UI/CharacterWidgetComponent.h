// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CharacterWidgetComponent.generated.h"

class UCharacterWidget;
class UEffect;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UCharacterWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);
	void SetManaPercent(float Percent);
	void SetRagePercent(float Percent);
	void UpdateEffectIcons(TArray<UEffect*> Effects);
	TArray<UEffectIcon*> GetEffectIconsAsArray();
	
	void ShowRageBar();
	void SetSpecialInfoText(FString DisplayedText);
	void ShowSpecialInfoText();
	void HideSpecialInfoText();
	void SetArrowToWhite();
	void SetArrowToColor(FColor Color);
	void HideRageBar();
	void ShowArrow();
	void HideArrow();

	UCharacterWidget* CharacterWidget;
};
