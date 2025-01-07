// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LevelUpButton.h"
#include "Characters/Ally.h"
#include "GameMechanics/AttributeComponent.h"

void ULevelUpButton::LevelUpStat()
{
	if (Ally == nullptr || Ally->Attributes == nullptr || Ally->AttributePoints < 1 || Attribute.IsEmpty()) return;
	int32 Stat = *Ally->Attributes->Stats.Find(Attribute);
	Ally->Attributes->Stats.Emplace(Attribute, ++Stat);
	Ally->AttributePoints--;
	OnStatLeveledUp.Broadcast();
}
