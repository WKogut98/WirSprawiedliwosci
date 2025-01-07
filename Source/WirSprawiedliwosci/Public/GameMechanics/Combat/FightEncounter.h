// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FightEncounter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFightEndedDelegate);
class UFight;
class AEnemy;
class ADefaultGameMode;
class USpringArmComponent;
class UCameraComponent;
class USceneComponent;
class UAudioComponent;
class UItem;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API AFightEncounter : public AActor
{
	GENERATED_BODY()
	
public:	
	AFightEncounter();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnFirstEnemy();

	UFUNCTION(BlueprintCallable)
	virtual void StartFight();

	void EndFightSuccess();
	void EndFightFailure();
	void SpawnEnemyParty();
	void SpawnEnemy(FVector& Location, TArray<TSubclassOf<AEnemy>> EnemyClassArray);
	void SetupAllies();
	void OffsetAllyLocation(FVector& Location);
	void OffestEnemySpawnLocation(FVector& Location);
	void SetupAlliesAfterFightEnd();
	void PlayBattleSong();
	void StopBattleSong();
	void PlaySuccessSound();
	void PlayFailSound();

	void SwitchToCombatOverlay();
	void SwitchToPartyOverlay();

	UFUNCTION(BlueprintCallable)
	void EnableMouseCursor();

	UFUNCTION(BlueprintCallable)
	void DisableMouseCursor();

	ADefaultGameMode* CurrentGameMode;

	UPROPERTY(EditAnywhere)
	int16 Level;

	UPROPERTY(EditAnywhere)
	bool bIndoors;

	UPROPERTY(EditAnywhere)
	int32 RespawnsLeft;

	AEnemy* SpawnedEnemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy> SpawnedEnemyClass;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AEnemy>> EnemyPartyClassesRankOne;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AEnemy>> EnemyPartyClassesRankTwo;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AEnemy>> EnemyPartyClassesRankThree;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AEnemy>> EnemyPartyClassesRankFour;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UItem>> ItemRewardClasses;

	UPROPERTY()
	UItem* ItemReward;

	UPROPERTY()
	UFight* Fight;

	int32 GainedExp = 0;

	UPROPERTY(EditAnywhere)
	float MoneyReward = 0.f;

	UPROPERTY(EditAnywhere, Category = Sound)
	TArray<USoundBase*> BattleSongs;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* SuccessSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* FailSound;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnFightEndedDelegate OnFightEnded;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	bool bUseYAxis = false;

	UPROPERTY(EditAnywhere)
	USceneComponent* MainComponent;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	UAudioComponent* MusicPlayer;

	FTimerHandle FightEndTimer;
};