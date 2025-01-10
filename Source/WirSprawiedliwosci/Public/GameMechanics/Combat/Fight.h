// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Fight.generated.h"

class ACombatCharacter;
class AAlly;
class AEnemy;
class AFightEncounter;
class ADefaultHUD;
class ADefaultGameMode;
class USkill;
class USummonSkill;
class UItem;
class APlayerController;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API UFight : public UObject
{
	GENERATED_BODY()
public:
	UFight();
	void BuildTurnOrder();

	UFUNCTION()
	void StartTurn();

	UFUNCTION()
	void EndTurn();

	void MarkTargetsForSelection();
	void MarkAllyTargetsForSelection();
	void MarkEnemyTargetsForSelection();
	bool SkillTargetsAlly();
	bool SkillTargetsEnemy();
	void AddCurrentCharacterToOverlay();
	void AddSkillIconClickedEvents();
	void AddItemClickedEvents();
	void PerformEnemyTurn();
	void ChooseAttackTarget();
	AAlly* GetFirstAliveAlly();
	bool NoTargetsForEnemyAttack();
	void TriggerMelee(ACombatCharacter* Source);
	void UseSkillMelee();
	void CharacterMoveToLocation();
	void ClearEnemyArrows();
	AEnemy* GetLowestHealthEnemy();
	AAlly* GetDefenderAlly();
	AEnemy* GetDefenderEnemy();
	AAlly* GetSpecificTarget();
	USkill* GetEnemyHealingSkill(AEnemy* CurrentEnemy);
	USkill* GetEnemySupportSkill(AEnemy* CurrentEnemy);
	USkill* GetEnemySummonSkill(AEnemy* CurrentEnemy);
	TArray<USkill*> GetEnemyAttacks(AEnemy* CurrentEnemy);
	TArray<ACombatCharacter*> BuildTargetArray();
	TArray<int32> GetAvailableAllyRanks();
	
	UFUNCTION()
	void HandleOnSkillIconClicked(USkill* ClickedSkill);

	UFUNCTION()
	void HandleOnEndTurnButtonClicked();

	UFUNCTION()
	void HandleOnCharacterHover(ACombatCharacter* HoveredCharacter);

	UFUNCTION()
	void HandleOnCharacterEndHover(ACombatCharacter* HoveredCharacter);

	UFUNCTION()
	void HandleOnCharacterClicked(ACombatCharacter* ClickedCharacter);

	void UseSkill();

	void UseSummonSkill(USummonSkill* Skill);

	UFUNCTION()
	void UseItem(UItem* Item);

	UFUNCTION()
	void HandleOnTargetReached(ACombatCharacter* Character);

	UFUNCTION()
	void HandleOnLocationReached(ACombatCharacter* Character);

	UFUNCTION()
	void HandleOnCharacterDeath(ACombatCharacter* DeadCharacter);

	void EnableMouseEvents();
	void DisableMouseEvents();

	inline static bool IsFaster(ACombatCharacter* A, ACombatCharacter* B);

	bool AllAlliesDead();

	UPROPERTY()
	APlayerController* Controller;

	UPROPERTY()
	ADefaultGameMode* CurrentGameMode;

	UPROPERTY()
	ACombatCharacter* CurrentCharacter;

	UPROPERTY()
	ACombatCharacter* CurrentTarget;

	UPROPERTY()
	TArray<ACombatCharacter*> Targets;

	UPROPERTY()
	USkill* CurrentSkill;

	UPROPERTY()
	TArray<AAlly*> AlliedParty;

	UPROPERTY()
	TArray<AEnemy*> EnemyParty;

	UPROPERTY()
	AFightEncounter* Encounter;

	UPROPERTY()
	ADefaultHUD* CurrentHUD;

	UPROPERTY()
	TArray<ACombatCharacter*> TurnQueue;

	UPROPERTY()
	FTimerHandle TurnTimer;

	UPROPERTY()
	bool bTurnInProgress = false;
};
