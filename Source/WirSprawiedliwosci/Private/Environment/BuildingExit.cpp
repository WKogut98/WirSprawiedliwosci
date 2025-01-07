// Fill out your copyright notice in the Description page of Project Settings.

#include "Environment/BuildingExit.h"
#include "Components/BoxComponent.h"
#include "Characters/MainCharacter.h"
#include "UI/DefaultHUD.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "GameMechanics/GameModes/DefaultGameInstance.h"
#include "Kismet/GameplayStatics.h"

ABuildingExit::ABuildingExit()
{
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBox->SetupAttachment(GetRootComponent());
}

void ABuildingExit::HandleTriggerOverlap(AActor* OtherActor)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD == nullptr || MainCharacter == nullptr) return;
	CurrentHUD->ShowInteractPrompt(TEXT("Wyjd\u017a"));
	MainCharacter->OverlappedActor = this;
}

void ABuildingExit::HandleEndTriggerOverlap(AActor* OtherActor)
{
	if (GetWorld() == nullptr || GetWorld()->GetFirstPlayerController() == nullptr) return;
	AHUD* HUD = GetWorld()->GetFirstPlayerController()->GetHUD();
	if (HUD == nullptr || OtherActor == nullptr) return;
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(HUD);
	if (CurrentHUD == nullptr || MainCharacter == nullptr) return;
	MainCharacter->OverlappedActor = nullptr;
	CurrentHUD->HideInteractPrompt();
}

void ABuildingExit::Interact_Implementation()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	AHUD* HUD = World->GetFirstPlayerController()->GetHUD();
	if (HUD == nullptr) return;
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(HUD);
	if (CurrentHUD)
	{
		CurrentHUD->HideInteractPrompt();
	}
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(World->GetFirstPlayerController()->GetCharacter());
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(this));
	if (MainCharacter == nullptr || CurrentGameMode == nullptr) return;
	CurrentGameMode->CurrentMode = ECurrentMode::ECM_Exploration;
	if (Target)
	{
		MainCharacter->SetActorLocation(Target->GetActorLocation());
		MainCharacter->SetControllerRotation(GetActorRotation());
		MainCharacter->SetActorRotation(FRotator::ZeroRotator);
		MainCharacter->Flip(false);
		CurrentGameMode->UpdateAlliesLocation();
		CurrentGameMode->GetWorldTimerManager().SetTimer(TeleportTimer, CurrentGameMode, &ADefaultGameMode::UpdateAlliesLocation, 0.2f);
	}

	SaveGame();
}

void ABuildingExit::SaveGame()
{
	UDefaultGameInstance* CurrentInstance = Cast<UDefaultGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (CurrentInstance == nullptr) return;
	CurrentInstance->SaveGame();
}

// Called when the game starts or when spawned
void ABuildingExit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

