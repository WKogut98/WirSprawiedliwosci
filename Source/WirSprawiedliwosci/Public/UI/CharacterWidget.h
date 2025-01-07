// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterWidget.generated.h"

class UProgressBar;
class UImage;
class UEffectIcon;
class UTextBlock;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API UCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void ShowRageBar();
	void HideRageBar();
	void ShowArrow();
	void SetArrowToWhite();
	void SetArrowToColor(FColor Color);
	void HideArrow();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SpecialInfoText;

	UPROPERTY(meta = (BindWidget))
	UImage* Arrow;

	UPROPERTY(meta = (BindWidget))
	UEffectIcon* EffectIcon1;

	UPROPERTY(meta = (BindWidget))
	UEffectIcon* EffectIcon2;

	UPROPERTY(meta = (BindWidget))
	UEffectIcon* EffectIcon3;

	UPROPERTY(meta = (BindWidget))
	UEffectIcon* EffectIcon4;

	UPROPERTY(meta = (BindWidget))
	UEffectIcon* EffectIcon5;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ManaBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* RageBar;

	UPROPERTY(EditAnywhere)
	UTexture2D* ArrowWhite;

	UPROPERTY(EditAnywhere)
	UTexture2D* ArrowGreen;

	UPROPERTY(EditAnywhere)
	UTexture2D* ArrowRed;
};
