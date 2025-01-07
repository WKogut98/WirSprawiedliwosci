#include "Characters/Enemy.h"
#include "GameMechanics/Combat/FightEncounter.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "GameMechanics/Effect.h"
#include "Components/SphereComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "UI/CharacterWidgetComponent.h"

AEnemy::AEnemy()
{
	OuterSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Outer Sphere"));
	OuterSphere->SetupAttachment(GetRootComponent());

	InnerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Inner Sphere"));
	InnerSphere->SetupAttachment(GetRootComponent());
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (CurrentGameMode && CurrentGameMode->CurrentMode == ECurrentMode::ECM_Combat)
	{
		//Flip(true);
	}
	else
	{
		OuterSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOuterSphereOverlap);
		InnerSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnInnerSphereOverlap);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(CurrentGameMode && CurrentGameMode->CurrentMode==ECurrentMode::ECM_Exploration)
	{
		if (TargetEscaped())
		{
			ReturnToSpawn();
		}
		else
		{
			ChaseTarget();
		}
	}
	if (CombatTarget)
	{
		SetActorRotation(CombatTarget->GetActorRotation());
	}
}

bool AEnemy::bHasBuffs()
{
	for (UEffect* Effect : Effects)
	{
		if (Effect->bHeal || (Effect->TurnMode == EEffectTurnMode::EETM_BuffDebuff && Effect->GetValue() > 0.f))
		{
			return true;
		}
	}
	return false;
}

void AEnemy::DisableSphereCollisions()
{
	InnerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OuterSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::SetArrowToColor()
{
	if (CharacterWidgetComponent)
	{
		CharacterWidgetComponent->SetArrowToColor(FColor::Red);
	}
}

void AEnemy::OnOuterSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag(TriggerTag))
	{
		CombatTarget = OtherActor;
	}
	ChaseTarget();
}

void AEnemy::OnInnerSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Ally")) && OtherActor && OtherActor->ActorHasTag(TriggerTag) && SpawnerEncounter)
	{
		SpawnerEncounter->StartFight();
	}
}

bool AEnemy::TargetEscaped()
{
	if (CombatTarget == nullptr) return true;
	const double DistanceFromTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();
	return DistanceFromTarget > ChaseRadius;
}

void AEnemy::ChaseTarget()
{
	if (CombatTarget == nullptr || CurrentGameMode->CurrentMode!=ECurrentMode::ECM_Exploration) return;
	//AIController = UAIBlueprintHelperLibrary::GetAIController(this);
	MoveToTarget(CombatTarget);
	GetWorldTimerManager().SetTimer(ChaseTimer, this, &AEnemy::ChaseTarget, 0.3f, false);
}

void AEnemy::ReturnToSpawn()
{
	//AIController = UAIBlueprintHelperLibrary::GetAIController(this);
	MoveToTarget(SpawnerEncounter);
	GetWorldTimerManager().ClearTimer(ChaseTimer);
}
