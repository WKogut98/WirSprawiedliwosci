// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Ally.h"
#include "Characters/MainCharacter.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "GameMechanics/Skill.h"
#include "GameMechanics/AttributeComponent.h"
#include "UI/CharacterWidgetComponent.h"
#include "UI/DefaultHUD.h"
#include "Items/Item.h"

AAlly::AAlly()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAlly::BeginPlay()
{
	Super::BeginPlay();
	CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(this));
}

void AAlly::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FollowPlayer();
}

void AAlly::SetExperience(int32 NewExperience)
{
	Super::SetExperience(NewExperience);
	if (Experience >= ExpToNextLevel)
	{
		LevelUp();
	}
}

void AAlly::LevelUp()
{
	Experience -= ExpToNextLevel;
	if(Attributes)
	{
		Attributes->SetLevel(Attributes->GetLevel() + 1);
		int level = Attributes->GetLevel();
		if (level < 4)
		{
			SkillPoints += 2;
		}
		else if (level >= 4 && level < 8)
		{
			SkillPoints += 3;
		}
		else
		{
			SkillPoints += 4;
		}
	}
	ExpToNextLevel += 100;
	AttributePoints += 2;
	PlayLevelUpSound();
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD)
	{
		CurrentHUD->ShowInfoText(TEXT("Nowy poziom!"));
	}
}

void AAlly::PlayLevelUpSound()
{
	if (LevelUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LevelUpSound, GetActorLocation());
	}
}

void AAlly::FollowPlayer()
{
	//GetWorldTimerManager().ClearTimer(FollowTimer);
	ACharacter* MainCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (CurrentGameMode == nullptr || MainCharacter==nullptr) return;
	if (this->IsA(AMainCharacter::StaticClass()) || !CurrentGameMode->AlliedParty.Contains(this)) return;
	if (AIController == nullptr)
	{
		AIController = UAIBlueprintHelperLibrary::GetAIController(this);
	}
	if (AIController == nullptr) return;
	SetActorRotation(MainCharacter->GetActorRotation());
	FAIMoveRequest MoveRequest;
	if (CurrentGameMode->CurrentMode == ECurrentMode::ECM_Exploration && GetDistanceTo(MainCharacter) >= (Rank-1)*80.f)
	{
		MoveRequest.SetGoalActor(MainCharacter);
		MoveRequest.SetAcceptanceRadius(5.f);
		AIController->MoveTo(MoveRequest);
	}
	else if (CurrentGameMode->CurrentMode == ECurrentMode::ECM_Combat && (bShouldMoveToTarget || bShouldMoveToLocation))
	{
		return;
	}
	else
	{
		AIController->StopMovement();
	}
	//GetWorldTimerManager().SetTimer(FollowTimer, this, &AAlly::FollowPlayer, 0.5f, true, 0.5f);
}

void AAlly::SetArrowToColor()
{
	if(CharacterWidgetComponent)
	{
		CharacterWidgetComponent->SetArrowToColor(FColor::Green);
	}
}

void AAlly::StartTurn()
{
	bCanUseItems = true;
	Super::StartTurn();
}

void AAlly::UseRandomFood()
{
	if (CurrentGameMode == nullptr || CurrentGameMode->Inventory.IsEmpty()) return;
	TArray<UItem*> FoodItems = CurrentGameMode->GetAllFoodItems();
	if (FoodItems.IsEmpty()) return;
	int32 Index = FMath::RandRange(0, FoodItems.Num() - 1);
	FoodItems[Index]->UseItem(this);
}

void AAlly::FlipDelayed(float Time)
{
	GetWorldTimerManager().SetTimer(this->FlipTimer, this, &AAlly::FlipDelayedCallback, Time);
}

void AAlly::FlipDelayedCallback()
{
	GetWorldTimerManager().ClearTimer(this->FlipTimer);
	Flip(false);
	SetActorLocation(InitialLocation);
}

ADefaultGameMode* AAlly::GetCurrentGameMode()
{
	if (CurrentGameMode) return CurrentGameMode;
	return nullptr;
}
