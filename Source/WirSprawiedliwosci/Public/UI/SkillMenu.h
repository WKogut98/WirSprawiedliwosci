// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillMenu.generated.h"

class USkillIcon;
class UTextBlock;
class AAlly;
class USkill;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API USkillMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetNewAlly(AAlly* NewAlly);

	void UpdateSkillPointText();

	void UpdateSkillIcons();
	void UpdateEquippedSkillIcons();

	UFUNCTION()
	void HandleSkillIconClicked(USkill* ClickedSkill);

	UFUNCTION()
	void HandleEquippedSkillIconClicked(USkill* ClickedSkill);

	TArray<USkillIcon*> GetSkillIconsAsArray();
	TArray<USkillIcon*> GetEquippedSkillIconsAsArray();

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon1Lvl1;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon1Lvl2;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon1Lvl3;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon1Lvl4;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon1Lvl5;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon2Lvl1;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon2Lvl2;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon2Lvl3;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon2Lvl4;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon2Lvl5;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon3Lvl1;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon3Lvl2;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon3Lvl3;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon3Lvl4;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon3Lvl5;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon4Lvl1;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon4Lvl2;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon4Lvl3;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon4Lvl4;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon4Lvl5;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon5Lvl1;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon5Lvl2;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon5Lvl3;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon5Lvl4;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon5Lvl5;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon6Lvl1;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon6Lvl2;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon6Lvl3;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon6Lvl4;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon6Lvl5;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon7Lvl1;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon7Lvl2;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon7Lvl3;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon7Lvl4;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon7Lvl5;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon8Lvl1;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon8Lvl2;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon8Lvl3;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon8Lvl4;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon8Lvl5;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon9Lvl1;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon9Lvl2;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon9Lvl3;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon9Lvl4;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* SkillIcon9Lvl5;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* EquippedSkill1;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* EquippedSkill2;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* EquippedSkill3;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* EquippedSkill4;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* EquippedSkillUlt;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillPointsText;

	UPROPERTY()
	AAlly* CurrentAlly;

	UPROPERTY()
	USkill* CurrentSkill;
};
