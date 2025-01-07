// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CombatCharacter.h"
#include "Enemy.generated.h"

class AFightEncounter;
class USphereComponent;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API AEnemy : public ACombatCharacter
{
	GENERATED_BODY()
public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;

	bool bHasBuffs();

	void DisableSphereCollisions();
	virtual void SetArrowToColor() override;

	UPROPERTY()
	AFightEncounter* SpawnerEncounter;

	UPROPERTY(EditAnywhere)
	FName TriggerTag = FName("MainCharacter");
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnOuterSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void OnInnerSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	bool TargetEscaped();
	void ChaseTarget();
	void ReturnToSpawn();

	UPROPERTY(VisibleAnywhere)
	USphereComponent* OuterSphere;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* InnerSphere;

	UPROPERTY(EditAnywhere)
	double ChaseRadius;

	FTimerHandle ChaseTimer;
};
