// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "CharacterSpawnPoint.generated.h"

class ASpriteCharacter;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API ACharacterSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:
	UPROPERTY()
	ASpriteCharacter* Character;
protected:
	virtual void BeginPlay() override;
};
