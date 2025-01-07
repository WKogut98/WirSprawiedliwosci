// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDetails.generated.h"

class UItem;
class UTextBlock;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UItemDetails : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetNewItem(UItem* NewItem);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Description;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Quote;
};
