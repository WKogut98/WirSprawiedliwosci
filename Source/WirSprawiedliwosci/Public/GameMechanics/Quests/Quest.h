// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Quest.generated.h"

class UObjectiveCollection;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API AQuest : public AActor
{
	GENERATED_BODY()
	
public:	
	AQuest();
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE UObjectiveCollection* GetRootObjectiveCollection() const { return RootObjectiveCollection; };
	FORCEINLINE void SetRootObjectiveCollection(UObjectiveCollection* NewRoot) { RootObjectiveCollection = NewRoot; };

	UFUNCTION(BlueprintCallable)
	void StartQuest();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UObjectiveCollection* InitializeRoot();

	UObjectiveCollection* InitializeRoot_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitializeObjectives();

	virtual void InitializeObjectives_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FText Description;
protected:
	virtual void BeginPlay() override;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UObjectiveCollection* RootObjectiveCollection;
};
