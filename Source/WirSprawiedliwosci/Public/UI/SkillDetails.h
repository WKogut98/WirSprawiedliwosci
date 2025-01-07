// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillDetails.generated.h"

class USkill;
class UTextBlock;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API USkillDetails : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetNewSkill(USkill* NewSkill);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Level;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Description;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Quote;
};
