#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameMechanics/GameModes/GameModes.h"
#include "DefaultGameMode.generated.h"

#pragma once

class ACombatCharacter;
class AAlly;
class AMainCharacter;
class AEnemy;
class UItem;
class AFightEncounter;
class AQuest;
class ACharacterSpawnPoint;
class UDataTable;
class UDefaultGameInstance;
struct FCharacterStatData;
struct FSkillData;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API ADefaultGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	void InitializeCharacter(ACombatCharacter* Character, FCharacterStatData* CharacterData);
	void InitializeAllySkills(AAlly* Character);
	void InitializeEnemySkills(ACombatCharacter* Character, int32 Level);
	void InitializeQuests();
	FCharacterStatData* FindAlly(ACombatCharacter* Character);
	FCharacterStatData* FindEnemy(ACombatCharacter* Character);
	FSkillData* FindSkill(FString SkillName, int32 Level);
	void AddToParty(AAlly* Ally, int32 Rank);
	
	TArray<UItem*> GetAllFoodItems();

	UFUNCTION(BlueprintCallable)
	void UpdateAlliesLocation();

	UFUNCTION(BlueprintCallable)
	void RemoveAlly(FString AllyName, bool bRemovePermanently);

	UFUNCTION(BlueprintCallable)
	void AddNewAlly(AAlly* NewAlly, int32 Rank);

	UDefaultGameInstance* CurrentInstance;

	UPROPERTY(EditAnywhere)
	UDataTable* AllyStatData;

	UPROPERTY(EditAnywhere)
	UDataTable* EnemyStatData;

	UPROPERTY(EditAnywhere)
	UDataTable* SkillData;

	UPROPERTY(BlueprintReadOnly)
	TArray<AAlly*> AvailableAllies;

	UPROPERTY(BlueprintReadWrite)
	TArray<AAlly*> AlliedParty;

	UPROPERTY(BlueprintReadOnly)
	TArray<UItem*> Inventory;

	UPROPERTY()
	TArray<AQuest*> Quests;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AQuest> QuestBaseClass;

	UPROPERTY(BlueprintReadWrite)
	float Money;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECurrentMode CurrentMode;

	//bool bCanChooseTargets;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//FName CurrentMapName;

	UPROPERTY(EditAnywhere)
	TArray<ACharacterSpawnPoint*> AllySpawnPoints;

	int32 FreeSpawnPointIndex = 0;
protected:
	virtual void BeginPlay() override;
	
	void InitializeAlliesFromTable(AMainCharacter* MainCharacter);
	void InitializeCharacterStats(ACombatCharacter* Character, FCharacterStatData* CharacterData);
	void InitializeCharacterResistances(ACombatCharacter* Character, FCharacterStatData* CharacterData);
	void SetCharacterHealthAndMana(ACombatCharacter* Character, FCharacterStatData* CharacterData);
	void SetCharacterExp(ACombatCharacter* Character, FCharacterStatData* CharacterData);
};
