// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMechanics/AttributeComponent.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "GameMechanics/InteractInterface.h"
#include "UI/CharacterWidgetComponent.h"
#include "UI/DefaultHUD.h"
#include "UI/InventoryWidget.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	//GetRootComponent()->SetRelativeScale3D(FVector(0.165799, 1.f, 0.165799));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-10.f, -90.f, 0.f));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	Attributes->SetRage(0.f);
	Attributes->SetMaxRage(10.f);
	if (CharacterWidgetComponent) CharacterWidgetComponent->ShowRageBar();
	UpdateRageBar();
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveUp"), this, &AMainCharacter::MoveUp);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("RotateCamera"), this, &AMainCharacter::RotateCamera);
	PlayerInputComponent->BindAction(FName("Interact"), IE_Pressed, this, &AMainCharacter::Interact);
	PlayerInputComponent->BindAction(FName("Fart"), IE_Pressed, this,  &AMainCharacter::Fart);
	PlayerInputComponent->BindAction(FName("OpenCloseInventory"), IE_Pressed, this, &AMainCharacter::OpenCloseInventory);
	PlayerInputComponent->BindAction(FName("OpenCharacterMenu"), IE_Pressed, this, &AMainCharacter::OpenCharacterMenu);
}

void AMainCharacter::SetControllerRotation(FRotator NewRotation)
{
	GetController()->SetControlRotation(NewRotation);
}

void AMainCharacter::MoveToTarget(AActor* Target)
{
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CombatTarget);
}

void AMainCharacter::MoveToLocation(const FVector& Location)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), TargetLocation);
}

void AMainCharacter::MoveRight(float Value)
{
	if (CurrentGameMode->CurrentMode == ECurrentMode::ECM_Combat) return;
	if (GetController() && (Value != 0.f))
	{
		const FVector ForwardVector = GetActorForwardVector();
		AddMovementInput(ForwardVector, Value);
	}
}

void AMainCharacter::MoveUp(float Value)
{
	if (CurrentGameMode->CurrentMode == ECurrentMode::ECM_Combat) return;
	if (GetController() && (Value != 0.f))
	{
		const FVector RightVector = GetActorRightVector();
		AddMovementInput(RightVector, Value);
	}
}

void AMainCharacter::RotateCamera(float Value)
{
	if (CurrentGameMode->CurrentMode == ECurrentMode::ECM_Combat || 
		CurrentGameMode->CurrentMode == ECurrentMode::ECM_ExplorationInterior) return;
	AddControllerYawInput(Value);
}

void AMainCharacter::Interact()
{
	if (OverlappedActor == nullptr) return;
	IInteractInterface* InteractInterface = Cast<IInteractInterface>(OverlappedActor);
	if (InteractInterface)
	{
		InteractInterface->Execute_Interact(OverlappedActor);
	}
}

void AMainCharacter::Fart()
{
	if (CurrentGameMode && CurrentGameMode->CurrentMode == ECurrentMode::ECM_Exploration && FartSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FartSound, GetActorLocation());
	}
}

void AMainCharacter::OpenCloseInventory()
{
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD == nullptr) return;
	CurrentHUD->ShowInventory();
}

void AMainCharacter::OpenCharacterMenu()
{
	if (!CurrentGameMode || CurrentGameMode->CurrentMode == ECurrentMode::ECM_Combat) return;
	ADefaultHUD* CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD == nullptr) return;
	CurrentHUD->ShowCharacterMenu();
}
