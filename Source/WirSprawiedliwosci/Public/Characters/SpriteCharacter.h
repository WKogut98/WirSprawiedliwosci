// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "CharacterStates.h"
#include "SpriteCharacter.generated.h"

class UPaperFlipbook;
class AAIController;
class ADefaultGameMode;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API ASpriteCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	ASpriteCharacter();
	//virtual void Tick(float DeltaTime) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Flip(bool bFaceLeft);

	UFUNCTION(BlueprintCallable)
	void ChangeFlipbook(FString ActionKey);
	bool InTargetRange(AActor* Target, float Radius);
	bool InPosition(const FVector& Location, float Radius);
	virtual void MoveToTarget(AActor* Target);
	virtual void MoveToLocation(const FVector& Location);

	virtual void Die();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CharacterName;

	UPROPERTY(EditAnywhere)
	UTexture2D* PortraitImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAlive;

	UPROPERTY()
	AAIController* AIController;

	//getters and setters
	FORCEINLINE TMap<FString, UPaperFlipbook*> GetActionSprites() const { return ActionSprites; };
protected:
	virtual void BeginPlay() override;

	void AnimateMovement();

	void AdjustOrientationToMovement();

	void PlayFootsteps();

	void PlayDeathSound();

	void ResetFootstepsLoop();

	UPROPERTY(EditAnywhere)
	bool bIsFacingLeft = false;

	bool bPlayFootsteps = true;

	FTimerHandle FootstepsLoop;

	UPROPERTY(EditAnywhere, Category=Sound)
	USoundBase* Footsteps;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = Sprite)
	TMap<FString, UPaperFlipbook*> ActionSprites;

	float RunningTime = 0.f;

	UPROPERTY(BlueprintReadOnly)
	ADefaultGameMode* CurrentGameMode;
private:

};
