#pragma once

#include "CoreMinimal.h"
#include "GameMechanics/Skill.h"
#include "SummonSkill.generated.h"

class AEnemy;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API USummonSkill : public USkill
{
	GENERATED_BODY()
public:
	USummonSkill();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AEnemy>> SummonedEnemyClasses;
};
