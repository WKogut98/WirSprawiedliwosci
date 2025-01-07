// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterIcon.generated.h"

class UImage;
class ASpriteCharacter;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UCharacterIcon : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetNewCharacter(ASpriteCharacter* NewCharacter);

	UPROPERTY(meta = (BindWidget))
	UImage* Portrait;

	ASpriteCharacter* Character;
};
