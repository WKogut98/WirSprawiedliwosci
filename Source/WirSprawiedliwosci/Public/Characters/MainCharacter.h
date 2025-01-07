// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Ally.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API AMainCharacter : public AAlly
{
	GENERATED_BODY()
public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetControllerRotation(FRotator NewRotation);

	virtual void MoveToTarget(AActor* Target) override;

	virtual void MoveToLocation(const FVector& Location) override;

	UPROPERTY(BlueprintReadWrite)
	AActor* OverlappedActor;
protected:
	virtual void BeginPlay() override;

	//input functions
	void MoveRight(float Value);
	void MoveUp(float Value);
	void RotateCamera(float Value);
	void Interact();
	void Fart();
	void OpenCloseInventory();
	void OpenCharacterMenu();

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* FartSound;
private:
};
