// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "GameMechanics/InteractInterface.h"
#include "Building.generated.h"

class UBoxComponent;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API ABuilding : public APaperSpriteActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	ABuilding();

	UFUNCTION(BlueprintCallable)
	void HandleTriggerOverlap(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void HandleEndTriggerOverlap(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	virtual void Interact_Implementation() override;

	void SaveGame();

	UPROPERTY(EditAnywhere)
	AActor* Target;

	//getters and setters
	FORCEINLINE bool IsAccessible() const { return bIsAccessible; };
	FORCEINLINE void SetIsAccessible(bool NewIsAccessible) { bIsAccessible = NewIsAccessible; };
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAccessible;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* InteractTrigger;

	FTimerHandle TeleportTimer;
private:	
};
