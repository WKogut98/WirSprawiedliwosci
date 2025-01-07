// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatOverlay.generated.h"

class USkillIcon;
class UCharacterIcon;
class UTextBlock;
class UButton;
class AAlly;
class AEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndTurnButtonClicked);
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UCombatOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetNewAlly(AAlly* NewAlly);
	void SetNewEnemy(AEnemy* NewEnemy);
	
	UFUNCTION(BlueprintCallable)
	void EndTurnButtonClicked();

	TArray<USkillIcon*> GetAllySkillIconsAsArray();
	TArray<USkillIcon*> GetEnemySkillIconsAsArray();

	AAlly* CurrentAlly;
	AEnemy* CurrentEnemy;

	UPROPERTY(meta = (BindWidget))
	UCharacterIcon* AllyPortrait;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AllyName;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* AllySkill1;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* AllySkill2;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* AllySkill3;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* AllySkill4;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* AllyUlt;

	UPROPERTY(meta = (BindWidget))
	UCharacterIcon* EnemyPortrait;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnemyName;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* EnemySkill1;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* EnemySkill2;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* EnemySkill3;

	UPROPERTY(meta = (BindWidget))
	USkillIcon* EnemySkill4;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton;

	UPROPERTY(meta = (BindWidget))
	UButton* EndTurnButton;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnEndTurnButtonClicked OnEndTurnButtonClicked;
};
