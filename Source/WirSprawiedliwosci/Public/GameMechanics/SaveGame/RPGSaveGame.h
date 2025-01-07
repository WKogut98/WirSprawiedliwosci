// Po kim ty taki chitry na prawa autorskie?!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DataStructures/AllyData.h"
#include "DataStructures/ItemData.h"
#include "DataStructures/WorldData.h"
#include "GameMechanics/GameModes/GameModes.h"
#include "GameFramework/SaveGame.h"
#include "DataStructures/QuestData.h"
#include "RPGSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class WIRSPRAWIEDLIWOSCI_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FAllyData> AlliesData;

	UPROPERTY()
	TArray<FItemData> InventoryData;

	UPROPERTY()
	float Money;

	UPROPERTY()
	ECurrentMode CurrentMode;

	UPROPERTY()
	TArray<FBuildingData> BuildingsData;

	UPROPERTY()
	TArray<FInteractibleObjData> InteractibleObjsData;

	UPROPERTY()
	TArray<FCutsceneTriggerData> CutsceneTriggersData;

	UPROPERTY()
	TArray<FFightEncounterData> FightEncountersData;

	UPROPERTY()
	TArray<FQuestData> ActiveQuestsData;

	UPROPERTY()
	TArray<FString> KilledNPCs;

	UPROPERTY()
	TArray<FNPCData> NPCsData;
};
