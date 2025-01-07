// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/InteractibleObject.h"
#include "Components/BoxComponent.h"
#include "UI/ObjectWidgetComponent.h"
#include "UI/InteractPrompt.h"
#include "UI/DefaultHUD.h"
#include "Characters/MainCharacter.h"

AInteractibleObject::AInteractibleObject()
{
	InteractTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Interact Trigger"));
	InteractTrigger->SetupAttachment(GetRootComponent());
}

void AInteractibleObject::BeginPlay()
{
}

void AInteractibleObject::ShowPrompt()
{
	if (bInteracted) return;
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD)
	{
		CurrentHUD->ShowInteractPrompt(InteractPromptText);
	}
}

void AInteractibleObject::HidePrompt()
{
	if (GetWorld() == nullptr)
	{
		return;
	}
	if (GetWorld()->GetFirstPlayerController() == nullptr)
	{
		return;
	}
	if (GetWorld()->GetFirstPlayerController()->GetHUD() == nullptr)
	{
		return;
	}
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD)
	{
		CurrentHUD->HideInteractPrompt();
	}
}

void AInteractibleObject::Interact_Implementation()
{
	HidePrompt();
}

void AInteractibleObject::HandleTriggerOverlap(AActor* OtherActor)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		ShowPrompt();
		MainCharacter->OverlappedActor = this;
	}
}

void AInteractibleObject::HandleEndTriggerOverlap(AActor* OtherActor)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::FromHex("#FF5F1F"), __FUNCTION__);
		}
		MainCharacter->OverlappedActor = nullptr;
	}
	HidePrompt();
}

