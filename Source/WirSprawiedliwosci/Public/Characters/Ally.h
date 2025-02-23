// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CombatCharacter.h"
#include "Ally.generated.h"

class USkill;
class ADefaultGameMode;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API AAlly : public ACombatCharacter
{
	GENERATED_BODY()
public:
	AAlly();
	virtual void Tick(float DeltaTime) override;
	void LevelUp();

	UFUNCTION(BlueprintCallable)
	void FollowPlayer();

	virtual void SetArrowToColor() override;

	virtual void StartTurn() override;

	void UseRandomFood();

	void FlipDelayed(float Time);
	void FlipDelayedCallback();

	UFUNCTION(BlueprintCallable)
	ADefaultGameMode* GetCurrentGameMode();

	UPROPERTY()
	TArray<USkill*> Skills;

	UPROPERTY()
	bool bCanUseItems = true;

	UPROPERTY()
	int32 RoundsDrank = 0;

	UPROPERTY()
	int32 SkillPoints = 0;

	UPROPERTY()
	int32 AttributePoints = 0;

	//getters and setters
	virtual void SetExperience(int32 NewExperience) override;
protected:
	virtual void BeginPlay() override;

	void PlayLevelUpSound();

	UPROPERTY(EditAnywhere)
	USoundBase* LevelUpSound;

	FTimerHandle FlipTimer;
private:
};
