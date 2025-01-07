// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterIcon.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"
#include "Characters/SpriteCharacter.h"

void UCharacterIcon::SetNewCharacter(ASpriteCharacter* NewCharacter)
{
	Character = NewCharacter;
	if(Character && Character->PortraitImage)
	{
		Portrait->SetBrushFromTexture(Character->PortraitImage, false);
	}
	else
	{
		Portrait->SetBrushFromTexture(nullptr, false);
	}
}
