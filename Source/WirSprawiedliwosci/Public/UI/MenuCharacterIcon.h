// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CharacterIcon.h"
#include "MenuCharacterIcon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterIconClickedDelegate, ASpriteCharacter*, ClickedCharacter);

UCLASS()
class WIRSPRAWIEDLIWOSCI_API UMenuCharacterIcon : public UCharacterIcon
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void HandleOnCharacterIconClicked();

	FOnCharacterIconClickedDelegate OnCharacterIconClicked;
};
