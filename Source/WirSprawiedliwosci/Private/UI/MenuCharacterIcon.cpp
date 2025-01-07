// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuCharacterIcon.h"

void UMenuCharacterIcon::HandleOnCharacterIconClicked()
{
	OnCharacterIconClicked.Broadcast(Character);
}
