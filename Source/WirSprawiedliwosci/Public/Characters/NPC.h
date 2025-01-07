// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SpriteCharacter.h"
#include "Engine/TargetPoint.h"
#include "GameMechanics/InteractInterface.h"
#include "NPC.generated.h"

/**
 * 
 */
UCLASS()
class WIRSPRAWIEDLIWOSCI_API ANPC : public ASpriteCharacter, public IInteractInterface
{
	GENERATED_BODY()
public:
	virtual void Interact_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATargetPoint* SpawnPoint;
};
