#include "GameMechanics/Combat/FightEncounter.h"
#include "GameMechanics/Combat/Fight.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "GameMechanics/AttributeComponent.h"
#include "GameMechanics/Skill.h"
#include "Characters/Enemy.h"
#include "Characters/Ally.h"
#include "Characters/MainCharacter.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UI/DefaultHUD.h"
#include "UI/CombatOverlay.h"
#include "Items/Item.h"

AFightEncounter::AFightEncounter()
{
	PrimaryActorTick.bCanEverTick = true;
	MainComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(MainComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	MusicPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("Music Player"));
	MusicPlayer->SetupAttachment(GetRootComponent());
}

void AFightEncounter::BeginPlay()
{
	Super::BeginPlay();

	MusicPlayer->Stop();
	if(Tags.IsEmpty())
	{
		SpawnFirstEnemy();
	}
}

void AFightEncounter::SpawnFirstEnemy() //spawns enemy who chases you
{
	if (GetWorld())
	{
		if(RespawnsLeft <= 0) return;
		SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(SpawnedEnemyClass, MainComponent->GetComponentLocation(), FRotator::ZeroRotator);
		if (SpawnedEnemy)
		{
			SpawnedEnemy->SpawnerEncounter = this;
			AAIController* NewAIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);
			NewAIController->Possess(SpawnedEnemy);
			SpawnedEnemy->AIController = NewAIController;
		}
	}
	CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(this));
}

void AFightEncounter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFightEncounter::StartFight()
{
	Fight = nullptr;
	if (RespawnsLeft <= 0) return;
	RespawnsLeft--;
	if(SpawnedEnemy)
	{
		SpawnedEnemy->Destroy();
	}
	Fight = NewObject<UFight>(this, UFight::StaticClass());
	Fight->Encounter = this;
	CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(this));
	if (CurrentGameMode == nullptr) return;
	Fight->CurrentGameMode = CurrentGameMode;
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	Fight->CurrentHUD = CurrentHUD;
	CurrentHUD->ShowInfoText(TEXT("WALKA!"));
	CurrentGameMode->CurrentMode = ECurrentMode::ECM_Combat;

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	Fight->Controller = Controller;
	Fight->AddItemClickedEvents();

	Controller->SetViewTargetWithBlend(this, 1.f);
	SpawnEnemyParty();

	SetupAllies();

	SwitchToCombatOverlay();
	Fight->BuildTurnOrder();
	Fight->StartTurn();
	EnableMouseCursor();
	PlayBattleSong();
}

void AFightEncounter::EndFightSuccess()
{
	StopBattleSong();
	PlaySuccessSound();
	CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(this));
	if (CurrentGameMode)
	{
		if (bIndoors)
		{
			CurrentGameMode->CurrentMode = ECurrentMode::ECM_ExplorationInterior;
		}
		else
		{
			CurrentGameMode->CurrentMode = ECurrentMode::ECM_Exploration;
		}
		SwitchToPartyOverlay();
		ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if(!ItemRewardClasses.IsEmpty())
		{
			int32 ItemIndex = FMath::RandRange(0, ItemRewardClasses.Num() - 1);
			ItemReward = NewObject<UItem>(GetOuter(), ItemRewardClasses[ItemIndex]);
			ItemReward->Quantity = 1;
			ItemReward->AddToInventory();
		}
		CurrentGameMode->Money += MoneyReward;
		if (CurrentHUD)
		{
			CurrentHUD->ShowVictoryDialogue(GainedExp, ItemReward, MoneyReward);
		}
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (MainCharacter == nullptr) return;
		if(MainCharacter->Ult)
		{
			MainCharacter->Ult->bUsable = true;
		}
		APlayerController* Controller = GetWorld()->GetFirstPlayerController();
		Controller->SetViewTargetWithBlend(MainCharacter, 1.f);
		GetWorldTimerManager().ClearTimer(Fight->EndFightTimer);
		GetWorldTimerManager().SetTimer(FightEndTimer, this, &AFightEncounter::SetupAlliesAfterFightEnd, 1.f);
		OnFightEnded.Broadcast();
	}
}

void AFightEncounter::EndFightFailure()
{
	StopBattleSong();
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD)
	{
		CurrentHUD->ShowGameOverDialogue();
	}
	UGameplayStatics::GetPlayerController(this, 0)->SetPause(true);
	PlayFailSound();
}

void AFightEncounter::SpawnEnemyParty()
{
	FVector Location = GetActorLocation();

	SpawnEnemy(Location, EnemyPartyClassesRankOne);
	OffestEnemySpawnLocation(Location);
	SpawnEnemy(Location, EnemyPartyClassesRankTwo);
	OffestEnemySpawnLocation(Location);
	SpawnEnemy(Location, EnemyPartyClassesRankThree);
	OffestEnemySpawnLocation(Location);
	SpawnEnemy(Location, EnemyPartyClassesRankFour);
}

void AFightEncounter::SpawnEnemy(FVector& Location, TArray<TSubclassOf<AEnemy>> EnemyClassArray)
{
	int32 RandomIndex;
	if (!EnemyClassArray.IsEmpty())
	{
		RandomIndex = FMath::RandRange(0, EnemyClassArray.Num()-1);
		FRotator Rotator;
		if (!bUseYAxis)
		{
			Rotator = FRotator(0.f, 180.f, 0.f);
		}
		else
		{
			Rotator = FRotator(0.f, -90.f, 0.f);
		}
		AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClassArray[RandomIndex],
			Location, Rotator);

		//used for melee skills to return to the initial location
		NewEnemy->InitialLocation = Location;
		if(NewEnemy && CurrentGameMode)
		{
			NewEnemy->Attributes->SetLevel(Level);
			CurrentGameMode->InitializeCharacter(NewEnemy, CurrentGameMode->FindEnemy(NewEnemy));
			CurrentGameMode->InitializeEnemySkills(NewEnemy, Level);
			FActorSpawnParameters ControllerSpawnParams;
			ControllerSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AAIController* NewAIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), NewEnemy->GetActorLocation(), FRotator::ZeroRotator, ControllerSpawnParams);
			NewAIController->Possess(NewEnemy);
			NewEnemy->AIController = NewAIController;
			NewEnemy->DisableSphereCollisions();
			NewEnemy->ShowWidget();
			NewEnemy->ChangeFlipbook(TEXT("IdleCombat"));
			Fight->EnemyParty.Add(NewEnemy);
			NewEnemy->Rank = Fight->EnemyParty.Find(NewEnemy) + 1;
			NewEnemy->OnCharacterHover.AddDynamic(Fight, &UFight::HandleOnCharacterHover);
			NewEnemy->OnCharacterEndHover.AddDynamic(Fight, &UFight::HandleOnCharacterEndHover);
			NewEnemy->OnCharacterClicked.AddDynamic(Fight, &UFight::HandleOnCharacterClicked);
			NewEnemy->OnTargetReached.AddDynamic(Fight, &UFight::HandleOnTargetReached);
			NewEnemy->OnLocationReached.AddDynamic(Fight, &UFight::HandleOnLocationReached);
			NewEnemy->OnCharacterDeath.AddDynamic(Fight, &UFight::HandleOnCharacterDeath);
		}
	}
}

void AFightEncounter::SetupAllies()
{
	Fight->AlliedParty.Empty();
	if (CurrentGameMode == nullptr) return;
	Fight->AlliedParty = CurrentGameMode->AlliedParty;
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MainCharacter)
	{
		MainCharacter->SetControllerRotation(GetActorRotation());
		MainCharacter->SetActorRotation(FRotator::ZeroRotator);
		MainCharacter->Flip(false);
		if (MainCharacter->Ult && (MainCharacter->Attributes == nullptr || MainCharacter->Attributes->GetRage() < MainCharacter->Attributes->GetMaxRage()))
		{
			MainCharacter->Ult->bUsable = false;
		}
		else if(MainCharacter->Ult)
		{
			MainCharacter->Ult->bUsable = true;
		}
	}
	FVector Location;
	if(!bUseYAxis)
	{
		Location = GetActorLocation() - FVector(200.f, 0.f, 0.f);
	}
	else
	{
		Location = GetActorLocation() - FVector(0.f, 200.f, 0.f);
	}
	for (AAlly* Ally : CurrentGameMode->AlliedParty)
	{
		Ally->SetActorLocation(Location);
		Ally->SetActorRotation(FRotator::ZeroRotator);
		Ally->InitialLocation = Location;
		
		Ally->Flip(false);
		Ally->ShowWidget();
		Ally->HideArrow();
		Ally->AddStatModifiers();
		Ally->UpdateEffectIcons();
		Ally->ChangeFlipbook(TEXT("IdleCombat"));
		Ally->OnCharacterHover.AddDynamic(Fight, &UFight::HandleOnCharacterHover);
		Ally->OnCharacterEndHover.AddDynamic(Fight, &UFight::HandleOnCharacterEndHover);
		Ally->OnCharacterClicked.AddDynamic(Fight, &UFight::HandleOnCharacterClicked);
		Ally->OnTargetReached.AddDynamic(Fight, &UFight::HandleOnTargetReached);
		Ally->OnLocationReached.AddDynamic(Fight, &UFight::HandleOnLocationReached);
		Ally->OnCharacterDeath.AddDynamic(Fight, &UFight::HandleOnCharacterDeath);
		OffsetAllyLocation(Location);
		Ally->FlipDelayed(0.2f);
	}
}

void AFightEncounter::OffsetAllyLocation(FVector& Location)
{
	if (bUseYAxis)
	{
		Location.Y -= 100.f;
		return;
	}
	Location.X -= 100.f;
}

void AFightEncounter::OffestEnemySpawnLocation(FVector& Location)
{
	if (bUseYAxis)
	{
		Location.Y += 100.f;
		return;
	}
	Location.X += 100.f;
}

void AFightEncounter::SetupAlliesAfterFightEnd()
{
	if (CurrentGameMode == nullptr) return;
	for (AAlly* Ally : CurrentGameMode->AlliedParty)
	{
		Ally->Turn = 0;
		Ally->HideWidget();
		Ally->ChangeFlipbook(TEXT("Idle"));
		Ally->RemoveStatModifiers();
		if (Ally->ActorHasTag(FName("Dead")))
		{
			Ally->bIsAlive = true;
			Ally->Tags.Remove(FName("Dead"));
			Ally->Attributes->SetHealth(Ally->Attributes->GetMaxHealth() / 2.f);
		}
		else
		{
			float NewHealth = FMath::Clamp(Ally->Attributes->GetHealth() + Ally->Attributes->GetMaxHealth() * 0.25f, 0.f, Ally->Attributes->GetMaxHealth());
			Ally->Attributes->SetHealth(NewHealth);
			float NewMana = FMath::Clamp(Ally->Attributes->GetMana() + Ally->Attributes->GetMaxMana() * 0.25f, 0.f, Ally->Attributes->GetMaxMana());
			Ally->Attributes->SetMana(NewMana);
			Ally->Attributes->SetRage(Ally->Attributes->GetRage()/2.f);
		}
		int32 AllyExp = Ally->GetExperience();
		Ally->SetExperience(AllyExp + GainedExp);
	}
	for (AAlly* Ally : CurrentGameMode->AvailableAllies)
	{
		if (!CurrentGameMode->AlliedParty.Contains(Ally))
		{
			int32 AllyExp = Ally->GetExperience();
			Ally->SetExperience(AllyExp + (GainedExp/2));
		}
	}
}

void AFightEncounter::PlayBattleSong()
{
	if (BattleSongs.IsEmpty()) return;
	USoundBase* BattleSong = BattleSongs[FMath::RandRange(0, BattleSongs.Num() - 1)];
	if(BattleSong)
	{
		MusicPlayer->SetSound(BattleSong);
		MusicPlayer->Play();
	}
}

void AFightEncounter::StopBattleSong()
{
	if (MusicPlayer->IsPlaying())
	{
		MusicPlayer->Stop();
	}
}

void AFightEncounter::PlaySuccessSound()
{
	if (SuccessSound)
	{
		UGameplayStatics::PlaySound2D(this, SuccessSound);
	}
}

void AFightEncounter::PlayFailSound()
{
	if (FailSound)
	{
		UGameplayStatics::PlaySound2D(this, FailSound);
	}
}

void AFightEncounter::SwitchToCombatOverlay()
{
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD)
	{
		CurrentHUD->SwitchToCombatOverlay();
	}
	if (Fight && CurrentHUD && CurrentHUD->CombatOverlay)
	{
		CurrentHUD->CombatOverlay->OnEndTurnButtonClicked.AddDynamic(Fight, &UFight::HandleOnEndTurnButtonClicked);
	}
}

void AFightEncounter::SwitchToPartyOverlay()
{
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD)
	{
		CurrentHUD->SwitchToPartyOverlay();
	}
}

void AFightEncounter::EnableMouseCursor()
{
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void AFightEncounter::DisableMouseCursor()
{
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
}
