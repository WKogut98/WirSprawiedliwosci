// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillIcon.generated.h"

class USkill;
class UImage;
class USkillDetails;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillIconClickedDelegate, USkill*, ClickedSkill);

UCLASS()
class WIRSPRAWIEDLIWOSCI_API USkillIcon : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnSkillIconClickedDelegate OnSkillIconClicked;

	void SetNewSkill(USkill* NewSkill);

	//icon switching
	void SetIconToDefault();
	void SetIconToHovered();
	void SetIconToClicked();

	//mouse event callbacks
	UFUNCTION(BlueprintCallable)
	void HandleOnHover();

	UFUNCTION(BlueprintCallable)
	void HandleOnEndHover();

	UFUNCTION(BlueprintCallable)
	void HandleOnClicked();

	UFUNCTION(BlueprintCallable)
	void HandleOnReleased();

	//skill detail box
	void CreateSkillDetails();
	void ShowSkillDetails();
	void HideSkillDetails();

	UPROPERTY(meta = (BindWidget))
	UImage* Icon;

	UPROPERTY()
	USkill* Skill;

	UPROPERTY(EditAnywhere, Category=Tooltip)
	TSubclassOf<USkillDetails> SkillDetailsClass;

	UPROPERTY()
	USkillDetails* SkillDetails;
};
