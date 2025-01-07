// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillTypes.h"
#include "Effect.generated.h"

class UNiagaraSystem;
UCLASS(BlueprintType, Blueprintable)
class WIRSPRAWIEDLIWOSCI_API UEffect : public UObject
{
	GENERATED_BODY()
public:
	UEffect();

	UPROPERTY(EditAnywhere)
	FString EffectName;

	UPROPERTY(EditAnywhere)
	EEffectType EffectType;

	UPROPERTY(EditAnywhere)
	EEffectTurnMode TurnMode;

	UPROPERTY(EditAnywhere)
	bool bHeal;

	UPROPERTY()
	bool bApplied = false;

	UPROPERTY(EditAnywhere)
	UTexture2D* EffectIcon;

	UPROPERTY(EditAnywhere)
	USoundBase* EffectSound;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* EffectVFX;

	//void SetNameAndTurnMode();

	//getters
	FORCEINLINE float GetValue() const { return Value; };
	FORCEINLINE uint8 GetDuration() const { return Duration; };
	FORCEINLINE FString GetAffectedAttribute() const { return AffectedAttribute; };

	//setters
	void SetValue(float NewValue);
	void SetDuration(uint8 NewDuration);
protected:
	UPROPERTY(BlueprintReadWrite)
	float Value;

	UPROPERTY(BlueprintReadWrite)
	uint8 Duration;

	UPROPERTY(EditAnywhere)
	FString AffectedAttribute;
};
