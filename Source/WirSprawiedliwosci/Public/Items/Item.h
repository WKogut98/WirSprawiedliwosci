// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Food UMETA(DisplayName = "Jedzenie"),
	EIT_Alcohol UMETA(DisplayName = "Alkohol"),
	EIT_Cigarettes UMETA(DisplayName = "Papierosy"),
	EIT_Artifact UMETA(DisplayName = "Artefakt"),
	EIT_Drug UMETA(DisplayName = "Narkotyk")
};

class UEffect;
class AAlly;
class UNiagaraSystem;
UCLASS(BlueprintType, Blueprintable)
class WIRSPRAWIEDLIWOSCI_API UItem : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void AddToInventory();
	void UseItem(AAlly* Ally);
	void ApplyStatBonuses(AAlly* Ally);
	void ApplyResistanceBonuses(AAlly* Ally);
	void ApplyDrunk(AAlly* Ally);
	void BuildDescription();
	void BuildDescriptionForStatBonuses();
	void BuildDescriptionForResistanceBonuses();
	void BuildDescriptionForEffect();
	void PlayUseSound();
	void SpawnItemVFX(AAlly* Ally);

	UPROPERTY(EditAnywhere)
	FString ItemName;

	UPROPERTY(EditAnywhere)
	EItemType ItemType;

	UPROPERTY(EditAnywhere)
	UTexture2D* ItemImage;

	UPROPERTY()
	FString Description;

	UPROPERTY(EditAnywhere)
	FString Quote;

	UPROPERTY(EditAnywhere)
	float Healing;

	UPROPERTY(EditAnywhere)
	float Mana;

	UPROPERTY(EditAnywhere)
	float Rage;

	UPROPERTY(EditAnywhere)
	int32 GainedExp;

	UPROPERTY(EditAnywhere)
	bool bRemovesNegativeEffects;

	UPROPERTY(EditAnywhere)
	bool bNeverDepletes = false;

	UPROPERTY(EditAnywhere)
	TMap<FString, int32> StatBonuses;

	UPROPERTY(EditAnywhere)
	TMap<FString, int32> ResistanceBonuses;

	UPROPERTY()
	UEffect* InflictedEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEffect> InflictedEffectClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEffect> DrunkEffectClass;

	UPROPERTY(EditAnywhere)
	FString InflictedEffectName;

	UPROPERTY(EditAnywhere)
	int32 InflictedEffectValue;

	UPROPERTY(EditAnywhere)
	int32 InflictedEffectDuration;

	UPROPERTY(EditAnywhere)
	int32 AlcoholVoltage;

	UPROPERTY(BlueprintReadWrite)
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxQuantity;

	UPROPERTY(EditAnywhere, Category=Sound)
	USoundBase* UseSound;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ItemVFX;
};
