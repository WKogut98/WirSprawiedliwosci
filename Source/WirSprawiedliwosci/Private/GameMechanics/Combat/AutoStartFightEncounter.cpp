// Po kim ty taki chitry na prawa autorskie?!

#include "GameMechanics/Combat/AutoStartFightEncounter.h"
#include "GameMechanics/CutsceneTrigger.h"

void AAutoStartFightEncounter::BeginPlay()
{
	if (StartCutsceneTrigger)
	{
		StartCutsceneTrigger->OnCutsceneEnded.AddDynamic(this, &AAutoStartFightEncounter::StartFight);
	}
}

void AAutoStartFightEncounter::StartFight()
{
	if (!bDelay)
	{
		Super::StartFight();
		return;
	}
	GetWorldTimerManager().SetTimer(DelayTimer, this, &AAutoStartFightEncounter::StartFightDelayed, delayTime);
}

void AAutoStartFightEncounter::StartFightDelayed()
{
	Super::StartFight();
}
