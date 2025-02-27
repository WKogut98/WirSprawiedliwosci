// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Building.h"
#include "Components/BoxComponent.h"
#include "Characters/MainCharacter.h"
#include "UI/DefaultHUD.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "GameMechanics/GameModes/DefaultGameInstance.h"
#include "Kismet/GameplayStatics.h"

ABuilding::ABuilding()
{
	InteractTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Interact Trigger"));
	InteractTrigger->SetupAttachment(GetRootComponent());
}

void ABuilding::BeginPlay()
{
}

void ABuilding::HandleTriggerOverlap(AActor* OtherActor)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD == nullptr || MainCharacter == nullptr) return;
	if (bIsAccessible)
	{
		CurrentHUD->ShowInteractPrompt(TEXT("Wejd\u017a"));
		MainCharacter->OverlappedActor = this;
	}
	else
	{
		CurrentHUD->ShowInfoText(TEXT("Zamkni\u0119te"));
	}
}

void ABuilding::HandleEndTriggerOverlap(AActor* OtherActor)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD == nullptr || MainCharacter == nullptr) return;
	MainCharacter->OverlappedActor = nullptr;
	CurrentHUD->HideInteractPrompt();
}

void ABuilding::Interact_Implementation()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(World->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD)
	{
		CurrentHUD->HideInteractPrompt();
	}
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(World->GetFirstPlayerController()->GetCharacter());
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(this));
	if (MainCharacter == nullptr || CurrentGameMode == nullptr) return;
	CurrentGameMode->CurrentMode = ECurrentMode::ECM_ExplorationInterior;
	if (Target)
	{
		MainCharacter->SetActorLocation(Target->GetActorLocation());
		MainCharacter->SetControllerRotation(FRotator::ZeroRotator);
		MainCharacter->SetActorRotation(FRotator::ZeroRotator);
		MainCharacter->Flip(false);
		CurrentGameMode->UpdateAlliesLocation();
		CurrentGameMode->GetWorldTimerManager().SetTimer(TeleportTimer, CurrentGameMode, &ADefaultGameMode::UpdateAlliesLocation, 0.2f);
	}

	OnEntered.Broadcast();
	SaveGame();
}

void ABuilding::SaveGame()
{
	UDefaultGameInstance* CurrentInstance = Cast<UDefaultGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (CurrentInstance == nullptr) return;
	CurrentInstance->SaveGame();
}


