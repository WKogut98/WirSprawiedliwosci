// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EffectIcon.generated.h"

class UEffect;
class UImage;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UEffectIcon : public UUserWidget
{
	GENERATED_BODY()
public:
	UEffect* Effect;
	void SetNewEffect(UEffect* NewEffect);
	void ClearIcon();
	
	UPROPERTY(meta = (BindWidget))
	UImage* Icon;
};
