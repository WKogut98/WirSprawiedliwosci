// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIRSPRAWIEDLIWOSCI_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

	UFUNCTION()
	void ReceiveDamage(float Value);

	UFUNCTION()
	void ReceiveHealing(float Value);

	UFUNCTION()
	void IncreaseRage();

	//getters
	FORCEINLINE float GetLevel() const { return Level; };
	FORCEINLINE float GetHealth() const { return Health; };
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealthPercent() const { return Health/MaxHealth; };

	FORCEINLINE float GetMana() const { return Mana; };
	FORCEINLINE float GetMaxMana() const { return MaxMana; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetManaPercent() const { return Mana/MaxMana; };

	FORCEINLINE float GetRage() const { return Rage; };
	FORCEINLINE float GetMaxRage() const { return MaxRage; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetRagePercent() const { return Rage/MaxRage; };	

	//setters
	UFUNCTION()
	void SetLevel(float NewLevel);

	UFUNCTION()
	void SetHealth(float NewHealth);

	UFUNCTION()
	void SetMaxHealth(float NewMaxHealth);

	UFUNCTION()
	void SetMana(float NewMana);

	UFUNCTION()
	void SetMaxMana(float NewMaxMana);

	UFUNCTION()
	void SetRage(float NewRage);

	UFUNCTION()
	void SetMaxRage(float NewMaxRage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<FString, uint8> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<FString, uint8> Resistances;
protected:
	virtual void BeginPlay() override;

	//Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Rage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxRage;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
