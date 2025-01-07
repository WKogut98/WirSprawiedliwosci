// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMechanics/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::SetLevel(float NewLevel)
{
	Level = NewLevel;
}

void UAttributeComponent::SetHealth(float NewHealth)
{
	Health = NewHealth;
}

void UAttributeComponent::SetMaxHealth(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
}

void UAttributeComponent::SetMana(float NewMana)
{
	Mana = NewMana;
}

void UAttributeComponent::SetMaxMana(float NewMaxMana)
{
	MaxMana = NewMaxMana;
}

void UAttributeComponent::SetRage(float NewRage)
{
	Rage = NewRage;
}

void UAttributeComponent::SetMaxRage(float NewMaxRage)
{
	MaxRage = NewMaxRage;
}

void UAttributeComponent::IncreaseRage()
{
	Rage += 2.f;
	Rage = FMath::Clamp(Rage, 0.f, MaxRage);
}

void UAttributeComponent::ReceiveDamage(float Value)
{
	Health -= Value;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
}

void UAttributeComponent::ReceiveHealing(float Value)
{
	Health += Value;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
}


