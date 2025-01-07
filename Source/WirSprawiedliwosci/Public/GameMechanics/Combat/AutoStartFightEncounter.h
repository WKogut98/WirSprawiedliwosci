// Po kim ty taki chitry na prawa autorskie?!

#pragma once

#include "CoreMinimal.h"
#include "GameMechanics/Combat/FightEncounter.h"
#include "AutoStartFightEncounter.generated.h"

class ACutsceneTrigger;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API AAutoStartFightEncounter : public AFightEncounter
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	virtual void StartFight() override;

	UFUNCTION()
	void StartFightDelayed();

	UPROPERTY(EditAnywhere)
	bool bDelay = false;

	UPROPERTY(EditAnywhere)
	float delayTime = 0.f;

	UPROPERTY(EditAnywhere)
	ACutsceneTrigger* StartCutsceneTrigger;

	UPROPERTY()
	FTimerHandle DelayTimer;

	//UPROPERTY(EditAnywhere)
	//ACutsceneTrigger* EndCutsceneTrigger;
};
