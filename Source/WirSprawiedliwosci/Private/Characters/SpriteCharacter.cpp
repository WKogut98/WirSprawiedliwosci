// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SpriteCharacter.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

ASpriteCharacter::ASpriteCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	if (GetSprite())
	{
		GetSprite()->SetRelativeScale3D(FVector(0.165799, 1.f, 0.165799));
		GetSprite()->SetLooping(false);
	}
}

void ASpriteCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentGameMode = Cast<ADefaultGameMode>(UGameplayStatics::GetGameMode(this));
}

void ASpriteCharacter::Tick(float DeltaTime)
{
	RunningTime += DeltaTime;
	if (GetCharacterMovement()->Velocity.Size2D() > 0)
	{
		AnimateMovement();
		PlayFootsteps();
	}
	else
	{
		AddActorWorldOffset(FVector::ZeroVector);
		if (GetSprite())
		{
			GetSprite()->SetRelativeRotation(FRotator::ZeroRotator);
		}
	}
	AdjustOrientationToMovement();
}

void ASpriteCharacter::ChangeFlipbook(FString ActionKey)
{
	if (!ActionSprites.Find(ActionKey)) return;
	if(GetSprite())
	{
		GetSprite()->SetFlipbook(*ActionSprites.Find(ActionKey));
		GetSprite()->SetLooping(false);
		GetSprite()->PlayFromStart();
	}
}

bool ASpriteCharacter::InTargetRange(AActor* Target, float Radius)
{
	if (Target == nullptr) return false;
	const double DistanceFromTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceFromTarget <= Radius;
}

bool ASpriteCharacter::InPosition(const FVector& Location, float Radius)
{
	const double DistanceFromLocation = (Location - GetActorLocation()).Size();
	return DistanceFromLocation <= Radius;
}

void ASpriteCharacter::Die()
{
	bIsAlive = false;
	ChangeFlipbook(TEXT("Death"));
	PlayDeathSound();
	Tags.Add(FName("Dead"));
}

void ASpriteCharacter::Flip(bool bFaceLeft)
{
	if (bIsFacingLeft == bFaceLeft)
	{
		return;
	}
	if (GetSprite())
	{
		FVector SpriteComponentScale = GetSprite()->GetRelativeScale3D();
		float NewX = SpriteComponentScale.X;
		if (bFaceLeft)
		{
			NewX = (-1) * SpriteComponentScale.X;
		}
		else
		{
			NewX = abs(SpriteComponentScale.X);
		}
		bIsFacingLeft = bFaceLeft;
		GetSprite()->SetRelativeScale3D(FVector(NewX, SpriteComponentScale.Y, SpriteComponentScale.Z));
	}
}

void ASpriteCharacter::AnimateMovement()
{
	float Offset = 3.5f * FMath::Sin(RunningTime * 20.f);
	float RotationOffset = 1.f * FMath::Sin(RunningTime * 20.f);
	if(GetSprite())
	{
		GetSprite()->AddRelativeRotation(FRotator(RotationOffset, 0.f, 0.f));
	}
	AddActorWorldOffset(FVector(0.f, 0.f, Offset));
}

void ASpriteCharacter::AdjustOrientationToMovement()
{
	bool bFaceLeft = bIsFacingLeft;
	if (GetCharacterMovement() == nullptr || UGameplayStatics::GetPlayerCameraManager(this, 0) == nullptr) return;
	FVector VelocityDirection = GetCharacterMovement()->Velocity.GetSafeNormal();
	FRotator CameraRotation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraRotation();

	if (VelocityDirection.X < 0.f)
	{
		bFaceLeft = true;
	}
	else if (VelocityDirection.X > 0.f)
	{
		bFaceLeft = false;
	}
	if (CameraRotation.Yaw > 0.f)
	{
		bFaceLeft = !bFaceLeft;
	}
	if (VelocityDirection.X==0.f)
	{
		bFaceLeft = bIsFacingLeft;
	}
	Flip(bFaceLeft);
}

void ASpriteCharacter::PlayFootsteps()
{
	if (Footsteps && bPlayFootsteps)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Footsteps, GetActorLocation());
		bPlayFootsteps = false;

		float Time = FMath::RandRange(0.3f, 0.5f);
		GetWorldTimerManager().SetTimer(FootstepsLoop, this, &ASpriteCharacter::ResetFootstepsLoop, Time, false);
	}
}

void ASpriteCharacter::PlayDeathSound()
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
}

void ASpriteCharacter::ResetFootstepsLoop()
{
	bPlayFootsteps = true;
}

void ASpriteCharacter::MoveToTarget(AActor* Target)
{
	if (AIController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(25.f);
	MoveRequest.SetUsePathfinding(true);
	AIController->MoveTo(MoveRequest);
}

void ASpriteCharacter::MoveToLocation(const FVector& Location)
{
	if (AIController == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Location);
	MoveRequest.SetAcceptanceRadius(0.f);
	MoveRequest.SetUsePathfinding(true);
	AIController->MoveTo(MoveRequest);
}
