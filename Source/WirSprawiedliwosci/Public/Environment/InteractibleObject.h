// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "GameMechanics/InteractInterface.h"
#include "InteractibleObject.generated.h"

class UBoxComponent;
class UObjectWidgetComponent;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API AInteractibleObject : public APaperSpriteActor, public IInteractInterface
{
	GENERATED_BODY()
public:
	AInteractibleObject();

	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void HandleTriggerOverlap(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void HandleEndTriggerOverlap(AActor* OtherActor);

	UPROPERTY()
	bool bInteracted = false;
protected:
	virtual void BeginPlay() override;

	void ShowPrompt();
	void HidePrompt();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UObjectWidgetComponent* PromptComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* InteractTrigger;

	UPROPERTY(EditAnywhere)
	FString InteractPromptText;
};
