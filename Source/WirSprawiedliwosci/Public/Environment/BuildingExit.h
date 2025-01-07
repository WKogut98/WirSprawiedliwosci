// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameMechanics/InteractInterface.h"
#include "BuildingExit.generated.h"

class UBoxComponent;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API ABuildingExit : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	ABuildingExit();
	
	UFUNCTION(BlueprintCallable)
	void HandleTriggerOverlap(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void HandleEndTriggerOverlap(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	virtual void Interact_Implementation() override;

	void SaveGame();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere)
	AActor* Target;

	UPROPERTY()
	FTimerHandle TeleportTimer;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
