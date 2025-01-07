// Po kim ty taki chitry na prawa autorskie?!


#include "GameMechanics/Combat/FightEncounterAfterObjective.h"
#include "GameMechanics/Quests/Quest.h"
#include "GameMechanics/Quests/QuestObjective.h"
#include "GameMechanics/Quests/ObjectiveCollection.h"

void AFightEncounterAfterObjective::HandleObjectiveCompleted(UQuestObjective* CompletedObjective)
{
	SpawnFirstEnemy();
}

void AFightEncounterAfterObjective::BeginPlay()
{
	if (!RequiredQuest || !RequiredQuest->IsValidLowLevel() || RequiredObjectiveName.ToString().IsEmpty())
	{
		return;
	}
	Objective = RequiredQuest->GetRootObjectiveCollection()->FindObjectiveByName(RequiredObjectiveName);
	if (!Objective)
	{
		return;
	}
	if (!Objective->bCompleted)
	{
		Objective->OnObjectiveCompleted.AddDynamic(this, &AFightEncounterAfterObjective::HandleObjectiveCompleted);
		return;
	}
	SpawnFirstEnemy();
}
