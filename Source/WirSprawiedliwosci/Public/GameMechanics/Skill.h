// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillTypes.h"
#include "Combat/DamageTypes.h"
#include "Skill.generated.h"

class UEffect;
class ACombatCharacter;

UCLASS(BlueprintType, Blueprintable)
class WIRSPRAWIEDLIWOSCI_API USkill : public UObject
{
	GENERATED_BODY()
public:
	USkill();
	void UseSkill(ACombatCharacter* NewSource, TArray<ACombatCharacter*> NewTargets);
	void ExecuteUseSkill();
	void ApplySpecialProperty();
	void UseSupportSkill(ACombatCharacter* Target);
	void UseOffensiveSkill(ACombatCharacter* Target);
	void LevelUp();
	void BuildDescription();
	void BuildDescriptionForEffect(UEffect* Effect);
	float CalculateValue(ACombatCharacter* Target);
	static bool DetermineSuccess(float Chance);
	FString GetSpecificTargetNameForDescription();
	void SetSourceFlipbook();
	void PlaySkillSound();

	UPROPERTY(EditAnywhere)
	ESkillType SkillType;

	UPROPERTY(EditAnywhere)
	FString SkillName;

	UPROPERTY(EditAnywhere)
	uint8 Level;

	UPROPERTY(EditAnywhere)
	uint8 NumberOfTargets;

	UPROPERTY(EditAnywhere)
	TArray<uint8> ValidFirstTargetRanks;

	UPROPERTY(EditAnywhere)
	FString ActionKey;

	UPROPERTY(EditAnywhere)
	FString SpecificTargetName;

	UPROPERTY(EditAnywhere)
	UTexture2D* SkillIcon;

	UPROPERTY(EditAnywhere)
	UTexture2D* SkillIconHovered;

	UPROPERTY(EditAnywhere)
	UTexture2D* SkillIconClicked;

	UPROPERTY(EditAnywhere)
	UTexture2D* SkillIconLocked;

	UPROPERTY(EditAnywhere)
	float AnimDuration;

	UPROPERTY()
	float MinValue;

	UPROPERTY()
	float MaxValue;

	UPROPERTY()
	float BaseAccuracy;

	UPROPERTY()
	float BaseCritRate;

	UPROPERTY()
	float StunRate;

	UPROPERTY()
	float SelfDamage;

	UPROPERTY()
	float SelfHeal;

	UPROPERTY()
	float ManaCost;

	UPROPERTY(EditAnywhere)
	bool bMelee;

	UPROPERTY()
	bool bUnlocked = false;

	UPROPERTY()
	bool bUsable = true;

	UPROPERTY(EditAnywhere)
	FString Description;

	UPROPERTY(EditAnywhere)
	FString Quote;

	//getters and setters
	FORCEINLINE UEffect* GetInflictedEffect() const { return InflictedEffect; };
	FORCEINLINE TArray<UEffect*> GetSelfEffects() const { return SelfEffects; };
	FORCEINLINE void SetInflictedEffect(UEffect* NewEffect) { InflictedEffect = NewEffect; };
	FORCEINLINE void SetSelfEffects(TArray<UEffect*> NewSelfEffects) { SelfEffects = NewSelfEffects; };
protected:
	UPROPERTY()
	UEffect* InflictedEffect;

	UPROPERTY()
	TArray<UEffect*> SelfEffects;

	UPROPERTY(EditAnywhere)
	bool bRemovesNegativeEffects;

	UPROPERTY(EditAnywhere)
	ESkillSpecialProperty SpecialProperty;

	UPROPERTY(EditAnywhere)
	EDamageType DamageType;

	UPROPERTY(EditAnywhere)
	USoundBase* SkillSound;

	UPROPERTY()
	ACombatCharacter* Source;

	UPROPERTY()
	TArray<ACombatCharacter*> Targets;

private:
	UPROPERTY()
	FTimerHandle SkillDelayTimer;
};
