// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMechanics/Effect.h"


UEffect::UEffect()
{
}

void UEffect::SetValue(float NewValue)
{
	Value = NewValue;
}

void UEffect::SetDuration(uint8 NewDuration)
{
	Duration = NewDuration;
}
