// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterMenu.generated.h"

class UMenuCharacterIcon;
class UTextBlock;
class UWidgetSwitcher;
class UAttributesMenu;
class UPartyMenu;
class USkillMenu;
class AAlly;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UCharacterMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetCharacterIcons();

	UFUNCTION()
	void HandleCharacterIconClicked(ASpriteCharacter* ClickedCharacter);

	void SetCurrentAlly(AAlly* NewAlly);

	TArray<UMenuCharacterIcon*> GetCharacterIconsAsArray();

	UPROPERTY()
	AAlly* CurrentAlly;

	//Character icons
	UPROPERTY(meta = (BindWidget))
	UMenuCharacterIcon* CharacterIcon1;

	UPROPERTY(meta = (BindWidget))
	UMenuCharacterIcon* CharacterIcon2;

	UPROPERTY(meta = (BindWidget))
	UMenuCharacterIcon* CharacterIcon3;

	UPROPERTY(meta = (BindWidget))
	UMenuCharacterIcon* CharacterIcon4;

	UPROPERTY(meta = (BindWidget))
	UMenuCharacterIcon* CharacterIcon5;

	UPROPERTY(meta = (BindWidget))
	UMenuCharacterIcon* CharacterIcon6;

	UPROPERTY(meta = (BindWidget))
	UMenuCharacterIcon* CharacterIcon7;

	UPROPERTY(meta = (BindWidget))
	UMenuCharacterIcon* CharacterIcon8;

	UPROPERTY(meta = (BindWidget))
	UMenuCharacterIcon* CurrentCharacterIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentCharacterName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentCharacterExp;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UWidgetSwitcher* Switcher;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UPartyMenu* PartyMenu;

	UPROPERTY(meta = (BindWidget))
	UAttributesMenu* AttributesMenu;

	UPROPERTY(meta = (BindWidget))
	USkillMenu* SkillsMenu;
};
