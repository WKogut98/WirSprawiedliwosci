// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMechanics/Combat/Fight.h"
#include "GameMechanics/Combat/FightEncounter.h"
#include "GameMechanics/Skill.h"
#include "GameMechanics/SummonSkill.h"
#include "Characters/CombatCharacter.h"
#include "Characters/Ally.h"
#include "Characters/Enemy.h"
#include "GameMechanics/AttributeComponent.h"
#include "GameMechanics/Effect.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "UI/DefaultHUD.h"
#include "UI/InventorySlot.h"
#include "UI/InventoryWidget.h"
#include "UI/CombatOverlay.h"
#include "UI/SkillIcon.h"
#include "Items/Item.h"
#include "Kismet/KismetStringLibrary.h"

UFight::UFight()
{
}

void UFight::BuildTurnOrder()
{
	TurnQueue.Empty();
	//TArray<ACombatCharacter*> BothParties;
	//BothParties.Append(AlliedParty);
	//BothParties.Append(EnemyParty);
	TurnQueue.Append(AlliedParty);
	TurnQueue.Append(EnemyParty);
	Algo::Sort(TurnQueue, UFight::IsFaster);
	/*for (ACombatCharacter* Character : BothParties)
	{
		TurnQueue.Enqueue(Character);
		UE_LOG(LogTemp, Log, TEXT("Added character to TurnQueue: %s"), *Character->GetName());
	}
	BothParties.Empty();*/
}

void UFight::StartTurn()
{
	if (Encounter)
	{
		Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
	}
	if (CurrentCharacter)
	{
		CurrentCharacter->GetWorldTimerManager().ClearTimer(CurrentCharacter->TurnTimer);
	}
	if (bTurnInProgress || TurnQueue.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to start turn while another is in progress"));
		return;
	}
	bTurnInProgress = true;
	CurrentSkill = nullptr;
	CurrentCharacter = nullptr;
	CurrentTarget = nullptr;
	Targets.Empty();
	//TurnQueue.Dequeue(CurrentCharacter);
	CurrentCharacter = TurnQueue[0];
	if (!CurrentCharacter || !Encounter)
	{
		EndTurn();
		return;
	}
	CurrentCharacter->OnCharacterEndTurn.AddDynamic(this, &UFight::HandleOnCharacterEndTurn);
	TurnQueue.Remove(CurrentCharacter);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, __FUNCTION__);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, CurrentCharacter->CharacterName);
	}
	UE_LOG(LogTemp, Warning, TEXT("[Fight] Started Turn for character %s"), *CurrentCharacter->GetName());
	if (TurnQueue.IsEmpty())
	{
		BuildTurnOrder();
	}
	AddCurrentCharacterToOverlay();
	if(!CurrentCharacter || !CurrentCharacter->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid CurrentCharacter in StartTurn"));
		EndTurn();
		return;
	}
	CurrentCharacter->SetArrowToWhite();
	CurrentCharacter->ShowArrow();
	CurrentCharacter->StartTurn();
	if (CurrentCharacter->ActorHasTag(FName("Stunned")))
	{
		CurrentCharacter->ReturnFromStun();
		EndTurn();
		return;
	}
	if (CurrentCharacter->ActorHasTag(FName("Dead")))
	{
		EndTurn();
		return;
	}
	if (Encounter && CurrentCharacter->IsA(AEnemy::StaticClass()))
	{
		Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
		Encounter->GetWorldTimerManager().SetTimer(TurnTimer, this, &UFight::PerformEnemyTurn, 2.f);
	}
}

void UFight::EndTurn()
{
	if (!CurrentCharacter || !Encounter) return;
	
	CurrentCharacter->GetWorldTimerManager().ClearTimer(CurrentCharacter->TurnTimer);
	Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
	DisableMouseEvents();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow , __FUNCTION__);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, CurrentCharacter->CharacterName);
	}
	UE_LOG(LogTemp, Warning, TEXT("[Fight] Started Turn for character %s"), *CurrentCharacter->GetName());
	if (TurnQueue.IsEmpty())
	{
		BuildTurnOrder();
	}
	float TimeToWait = 0.5f;
	if (CurrentSkill)
	{
		TimeToWait = CurrentSkill->AnimDuration + 0.3f;
	}
	if (CurrentHUD && CurrentHUD->CombatOverlay)
	{
		CurrentHUD->CombatOverlay->SetNewAlly(nullptr);
	}
	bTurnInProgress = false;
	CurrentCharacter->UpdateEffectIcons();
	CurrentCharacter->GetWorldTimerManager().SetTimer(CurrentCharacter->TurnTimer, 
		CurrentCharacter, &ACombatCharacter::EndTurn, TimeToWait);
	for (AAlly* Ally : AlliedParty)
	{
		Ally->HideArrow();
	}
	for (AEnemy* Enemy : EnemyParty)
	{
		Enemy->HideArrow();
	}
	CurrentCharacter->HideArrow();
}

void UFight::MarkTargetsForSelection()
{
	MarkAllyTargetsForSelection();
	MarkEnemyTargetsForSelection();
}

void UFight::MarkAllyTargetsForSelection()
{
	bool bSkillHasSpecificTarget = false;
	AAlly* SpecificTarget = GetSpecificTarget();
	if (SpecificTarget)
	{
		bSkillHasSpecificTarget = true;
	}
	for (int i = 0; i < AlliedParty.Num(); i++)
	{
		if (SkillTargetsAlly() && CurrentSkill->ValidFirstTargetRanks.Contains(i + 1) && !AlliedParty[i]->ActorHasTag(FName("Dead")))
		{
			if (bSkillHasSpecificTarget && SpecificTarget->CharacterName != AlliedParty[i]->CharacterName)
			{
				AlliedParty[i]->bSelectable = false;
			}
			else
			{
				AlliedParty[i]->bSelectable = true;
			}
		}
		else
		{
			AlliedParty[i]->bSelectable = false;
		}
	}
}

void UFight::MarkEnemyTargetsForSelection()
{
	AEnemy* EnemyDefender = GetDefenderEnemy();
	if (EnemyDefender)
	{
		for (int i = 0; i < EnemyParty.Num(); i++)
		{
			EnemyParty[i]->bSelectable = false;
		}
		EnemyDefender->bSelectable = true;
		return;
	}
	for (int i = 0; i < EnemyParty.Num(); i++)
	{
		if (SkillTargetsEnemy() && CurrentSkill->ValidFirstTargetRanks.Contains(EnemyParty[i]->Rank))
		{
			EnemyParty[i]->bSelectable = true;
		}
		else
		{
			EnemyParty[i]->bSelectable = false;
		}
	}
}

bool UFight::SkillTargetsAlly()
{
	return CurrentSkill->SkillType == ESkillType::EST_Heal || CurrentSkill->SkillType == ESkillType::EST_Support || CurrentSkill->SkillType == ESkillType::EST_AttackAlly
		|| CurrentSkill->SkillType == ESkillType::EST_UltSupport;
}

bool UFight::SkillTargetsEnemy()
{
	return CurrentSkill->SkillType == ESkillType::EST_Attack || CurrentSkill->SkillType == ESkillType::EST_UltAttack;
}

void UFight::AddCurrentCharacterToOverlay()
{
	if (TurnQueue.IsEmpty()) return;
	if (CurrentCharacter==nullptr || !CurrentCharacter->IsValidLowLevel()) return;
	CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD && CurrentHUD->CombatOverlay)
	{
		AAlly* CurrentAlly = Cast<AAlly>(CurrentCharacter);
		if (CurrentAlly->IsValidLowLevel())
		{
			CurrentHUD->CombatOverlay->SetNewAlly(CurrentAlly);
			AddSkillIconClickedEvents();
		}
		AEnemy* CurrentEnemy = Cast<AEnemy>(CurrentCharacter);
		if (CurrentEnemy->IsValidLowLevel())
		{
			CurrentHUD->CombatOverlay->SetNewEnemy(CurrentEnemy);
		}
	}
}

void UFight::AddSkillIconClickedEvents()
{
	CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!CurrentHUD) return;
	TArray<USkillIcon*> SkillIcons = CurrentHUD->CombatOverlay->GetAllySkillIconsAsArray();
	for (USkillIcon* SkillIcon : SkillIcons)
	{
		if (SkillIcon && SkillIcon->Skill)
		{
			SkillIcon->OnSkillIconClicked.AddUniqueDynamic(this, &UFight::HandleOnSkillIconClicked);
			UE_LOG(LogTemp, Log, TEXT("Added skill icon clicked event for skill: %s"), *SkillIcon->Skill->SkillName);
		}
	}
	CurrentHUD->CombatOverlay->AllyUlt->OnSkillIconClicked.AddUniqueDynamic(this, &UFight::HandleOnSkillIconClicked);
}

void UFight::AddItemClickedEvents()
{
	if (!CurrentHUD) return;
	for (UInventorySlot* ItemSlot : CurrentHUD->InventoryWidget->InventorySlots)
	{
		ItemSlot->OnItemClicked.AddUniqueDynamic(this, &UFight::UseItem);
	}
}

void UFight::PerformEnemyTurn()
{
	if (Encounter)
	{
		Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
	}
	if (CurrentCharacter)
	{
		CurrentCharacter->GetWorldTimerManager().ClearTimer(CurrentCharacter->TurnTimer);
	}
	if (Encounter== nullptr || CurrentCharacter == nullptr)
	{
		EndTurn();
		return;
	}
	AEnemy* CurrentEnemy = Cast<AEnemy>(CurrentCharacter);
	Encounter->GetWorldTimerManager().SetTimer(TurnTimer, this, &UFight::EndTurn, 15.f);
	if (CurrentEnemy)
	{
		USkill* SummonSkill = GetEnemySummonSkill(CurrentEnemy);
		if (SummonSkill && CurrentEnemy->Attributes->GetHealthPercent() <= 0.4 && EnemyParty.Num()<=3)
		{
			CurrentTarget = CurrentEnemy;
			CurrentSkill = SummonSkill;
			Targets = BuildTargetArray();
			Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
			UseSkill();
			EndTurn();
		}
		AEnemy* LowestHealthEnemy = GetLowestHealthEnemy();
		USkill* HealingSkill = GetEnemyHealingSkill(CurrentEnemy);
		if (LowestHealthEnemy->Attributes->GetHealthPercent()<=0.35f && HealingSkill)
		{
			CurrentTarget = LowestHealthEnemy;
			CurrentSkill = HealingSkill;
			Targets = BuildTargetArray();
			if (CurrentSkill->bMelee)
			{
				TriggerMelee(CurrentEnemy);
			}
			else
			{
				Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
				UseSkill();
				EndTurn();
			}
			return;
		}
		USkill* SupportSkill = GetEnemySupportSkill(CurrentEnemy);
		if (CurrentEnemy->Turn<=3 && !CurrentEnemy->bHasBuffs() && SupportSkill &&
			!(SupportSkill->GetInflictedEffect() && SupportSkill->GetInflictedEffect()->bHeal
				&& LowestHealthEnemy->Attributes->GetHealth() == LowestHealthEnemy->Attributes->GetMaxHealth()))
		{
			CurrentSkill = SupportSkill;
			int32 Rank = 
				CurrentSkill->ValidFirstTargetRanks.IsEmpty() ? EnemyParty.Find(CurrentEnemy)+1 :
				FMath::RandRange(CurrentSkill->ValidFirstTargetRanks[0], CurrentSkill->ValidFirstTargetRanks[CurrentSkill->ValidFirstTargetRanks.Num() - 1]);
			if ((Rank - 1) < EnemyParty.Num())
			{
				CurrentTarget = EnemyParty[Rank - 1];
			}
			Targets = BuildTargetArray();
			if (CurrentSkill->bMelee)
			{
				TriggerMelee(CurrentEnemy);
			}
			else
			{
				Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
				UseSkill();
				EndTurn();
			}
			return;
		}
		TArray<USkill*> EnemyAttacks = GetEnemyAttacks(CurrentEnemy);
		if (!EnemyAttacks.IsEmpty())
		{
			int32 Index = FMath::RandRange(0, EnemyAttacks.Num() - 1);
			CurrentSkill = EnemyAttacks[Index];
			ChooseAttackTarget();
			if (CurrentTarget == nullptr)
			{
				EndTurn();
				return;
			}
			UEffect* InflictedEffect = CurrentSkill->GetInflictedEffect();
			if (InflictedEffect)
			{
				UEffect* EffectOfType = CurrentTarget->GetEffectOfType(InflictedEffect->EffectType);
				if (EffectOfType && EffectOfType->GetDuration() >= 2)
				{
					EnemyAttacks.Remove(CurrentSkill);
					if (EnemyAttacks.IsEmpty())
					{
						Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
						EndTurn();
						return;
					}
					Index = FMath::RandRange(0, EnemyAttacks.Num() - 1);
					CurrentSkill = EnemyAttacks[Index];
					ChooseAttackTarget();
				}
			}
			if (NoTargetsForEnemyAttack())
			{
				EnemyAttacks.Remove(CurrentSkill);
				if (EnemyAttacks.IsEmpty())
				{
					EndTurn();
					return;
				}
				Index = FMath::RandRange(0, EnemyAttacks.Num() - 1);
				CurrentSkill = EnemyAttacks[Index];
				if (NoTargetsForEnemyAttack())
				{
					Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
					EndTurn();
					return;
				}
				ChooseAttackTarget();
			}
			Targets = BuildTargetArray();
			if (CurrentSkill->bMelee)
			{
				TriggerMelee(CurrentEnemy);
			}
			else
			{
				Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
				UseSkill();
				EndTurn();
			}
			return;
		}
	}
	else
	{
		Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
		EndTurn();
		return;
	}
}

void UFight::ChooseAttackTarget()
{
	AAlly* AllyDefender = GetDefenderAlly();
	if (AllyDefender)
	{
		CurrentTarget = AllyDefender;
		return;
	}
	if (NoTargetsForEnemyAttack())
	{
		EndTurn();
		return;
	}
	TArray<int32> AvailableAllyRanks = GetAvailableAllyRanks();
	if (AvailableAllyRanks.IsEmpty())
	{
		EndTurn();
		return;
	}
	else
	{
		int32 Index = FMath::RandRange(0, AvailableAllyRanks.Num() - 1);
		if (AvailableAllyRanks[Index] < AlliedParty.Num() && AlliedParty[AvailableAllyRanks[Index]])
		{
			CurrentTarget = AlliedParty[AvailableAllyRanks[Index]];
		}
		else
		{
			ChooseAttackTarget();
		}
	}
}

AAlly* UFight::GetFirstAliveAlly()
{
	for (AAlly* Ally : AlliedParty)
	{
		if (Ally->ActorHasTag(FName("Dead")))
		{
			return Ally;
		}
	}
	return nullptr;
}

bool UFight::NoTargetsForEnemyAttack()
{
	for (AAlly* Ally : AlliedParty)
	{
		if (CurrentSkill && CurrentSkill->ValidFirstTargetRanks.Contains(AlliedParty.Find(Ally)) && !Ally->ActorHasTag(FName("Dead")))
		{
			return false;
		}
	}
	return true;
}

void UFight::TriggerMelee(ACombatCharacter* Source)
{
	if (!Encounter) return;
	if (Targets.IsEmpty())
	{
		Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
		EndTurn();
		return;
	}
	if(Targets[0] != Source)
	{
		Source->CombatTarget = Targets[0];
		Source->TargetLocation = Source->InitialLocation;
		Source->bShouldMoveToTarget = true;
	}
	else
	{
		Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
		UseSkill();
		EndTurn();
		return;
	}
}

AEnemy* UFight::GetLowestHealthEnemy()
{
	if (EnemyParty.IsEmpty()) return nullptr;
	AEnemy* LowestHealthEnemy = EnemyParty[0];
	for (int i = 1; i < EnemyParty.Num(); i++)
	{
		if (i < EnemyParty.Num() && EnemyParty[i] && EnemyParty[i]->Attributes->GetHealth() < EnemyParty[i - 1]->Attributes->GetHealth())
		{
			LowestHealthEnemy = EnemyParty[i];
		}
	}
	return LowestHealthEnemy;
}

AAlly* UFight::GetDefenderAlly()
{
	if (AlliedParty.IsEmpty())
	{
		return nullptr;
	}
	for (int i = 0; i < AlliedParty.Num(); i++)
	{
		if (AlliedParty[i] && AlliedParty[i]->ActorHasTag(FName("Defender")) && !AlliedParty[i]->ActorHasTag(FName("Dead")))
		{
			return AlliedParty[i];
		}
	}
	return nullptr;
}

AEnemy* UFight::GetDefenderEnemy()
{
	if (EnemyParty.IsEmpty()) return nullptr;
	for (int i = 0; i < EnemyParty.Num(); i++)
	{
		if (EnemyParty[i] && EnemyParty[i]->ActorHasTag(FName("Defender")))
		{
			return EnemyParty[i];
		}
	}
	return nullptr;
}

AAlly* UFight::GetSpecificTarget()
{
	for (int i = 1; i < AlliedParty.Num(); i++)
	{
		if (CurrentSkill && AlliedParty[i] && AlliedParty[i]->CharacterName == CurrentSkill->SpecificTargetName)
		{
			return AlliedParty[i];
		}
	}
	return nullptr;
}

USkill* UFight::GetEnemyHealingSkill(AEnemy* CurrentEnemy)
{
	if (CurrentEnemy->EquippedSkills.IsEmpty()) return nullptr;
	for (USkill* Skill : CurrentEnemy->EquippedSkills)
	{
		if (Skill->SkillType == ESkillType::EST_Heal && Skill->ManaCost <= CurrentCharacter->Attributes->GetMana())
		{
			return Skill;
		}
	}
	return nullptr;
}

USkill* UFight::GetEnemySupportSkill(AEnemy* CurrentEnemy)
{
	if (CurrentEnemy->EquippedSkills.IsEmpty()) return nullptr;
	for (USkill* Skill : CurrentEnemy->EquippedSkills)
	{
		if ((Skill->SkillType == ESkillType::EST_Support || Skill->SkillType == ESkillType::EST_AttackAlly)
			&& Skill->ManaCost <= CurrentCharacter->Attributes->GetMana())
		{
			return Skill;
		}
	}
	return nullptr;
}

USkill* UFight::GetEnemySummonSkill(AEnemy* CurrentEnemy)
{
	if (CurrentEnemy->EquippedSkills.IsEmpty()) return nullptr;

	for (USkill* Skill : CurrentEnemy->EquippedSkills)
	{
		if ((Skill->SkillType == ESkillType::EST_Summon)
			&& Skill->ManaCost <= CurrentCharacter->Attributes->GetMana())
		{
			return Skill;
		}
	}
	return nullptr;
}

TArray<USkill*> UFight::GetEnemyAttacks(AEnemy* CurrentEnemy)
{
	TArray<USkill*> EnemyAttacks;

	if (CurrentEnemy->EquippedSkills.IsEmpty()) return EnemyAttacks;

	for (USkill* Skill : CurrentEnemy->EquippedSkills)
	{
		if (Skill->SkillType == ESkillType::EST_Attack && Skill->SelfDamage<CurrentEnemy->Attributes->GetHealth()
			&& Skill->ManaCost <= CurrentCharacter->Attributes->GetMana())
		{
			EnemyAttacks.AddUnique(Skill);
		}
	}
	return EnemyAttacks;
}

TArray<ACombatCharacter*> UFight::BuildTargetArray()
{
	TArray<ACombatCharacter*> NewTargets;
	if (CurrentCharacter == nullptr || CurrentSkill == nullptr || CurrentTarget == nullptr) return NewTargets;
	if (CurrentSkill->NumberOfTargets == 0)
	{
		NewTargets = { CurrentCharacter };
		return NewTargets;
	}
	if(CurrentTarget->IsA(AAlly::StaticClass()))
	{
		AAlly* AllyTarget = Cast<AAlly>(CurrentTarget);
		int32 Index = AlliedParty.Find(AllyTarget);
		for (int32 i = Index; i < Index + CurrentSkill->NumberOfTargets; i++)
		{
			if (i < AlliedParty.Num() && AlliedParty[i] && !AlliedParty[i]->ActorHasTag(FName("Dead")))
			{
				NewTargets.Add(AlliedParty[i]);
			}
		}
	}
	if (CurrentTarget->IsA(AEnemy::StaticClass()))
	{
		AEnemy* EnemyTarget = Cast<AEnemy>(CurrentTarget);
		int32 Index = EnemyParty.Find(EnemyTarget);
		for (int32 i = Index; i < Index + CurrentSkill->NumberOfTargets; i++)
		{
			if (i < EnemyParty.Num())
			{
				NewTargets.Add(EnemyParty[i]);
			}
		}
	}
	return NewTargets;
}

TArray<int32> UFight::GetAvailableAllyRanks()
{
	TArray<int32> AliveAllyRanks;
	for (AAlly* Ally : AlliedParty)
	{
		int32 AllyIndex = AlliedParty.Find(Ally);
		if (CurrentSkill && CurrentSkill->ValidFirstTargetRanks.Contains(AllyIndex + 1) 
			&& !Ally->ActorHasTag(FName("Dead")))
		{
			AliveAllyRanks.AddUnique(AllyIndex);
		}
	}
	return AliveAllyRanks;
}

void UFight::HandleOnSkillIconClicked(USkill* ClickedSkill)
{
	if (!Encounter) return;
	EnableMouseEvents();
	if (ClickedSkill)
	{
		CurrentSkill = ClickedSkill;
		MarkTargetsForSelection();
		if (CurrentSkill->ManaCost > CurrentCharacter->Attributes->GetMana())
		{
			CurrentCharacter->ShowSpecialInfoText(TEXT("Za ma\u0142o many!"));
			DisableMouseEvents();
			return;
		}
		if (ClickedSkill->NumberOfTargets == 0)
		{
			Targets = { CurrentCharacter };
			UseSkill();
			DisableMouseEvents();
			Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
			EndTurn();
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Skill icon clicked: %s, Level: %d, Unlocked: %s"), *ClickedSkill->SkillName, ClickedSkill->Level, ClickedSkill->bUnlocked ? TEXT("true") : TEXT("false"));
	}
}

void UFight::HandleOnEndTurnButtonClicked()
{
	if (CurrentCharacter && !CurrentCharacter->IsA(AEnemy::StaticClass()))
	{
		EndTurn();
	}
}

void UFight::HandleOnCharacterHover(ACombatCharacter* HoveredCharacter)
{
	if (HoveredCharacter == nullptr || CurrentSkill == nullptr) return;
	if (CurrentSkill->NumberOfTargets <= 1) return;
	if (HoveredCharacter->IsA(AAlly::StaticClass()))
	{
		int32 Index = AlliedParty.Find(Cast<AAlly>(HoveredCharacter));
		for (int i = Index + 1; i < Index + CurrentSkill->NumberOfTargets; i++)
		{
			if (i >= AlliedParty.Num()) return;
			AlliedParty[i]->SetArrowToColor();
			AlliedParty[i]->ShowArrow();
		}
	}
	else
	{
		AEnemy* CurrentEnemy = Cast<AEnemy>(HoveredCharacter);
		if (!CurrentEnemy->IsValidLowLevel()) return;
		//AEnemy* EnemyDefender = GetDefenderEnemy();
		//if (EnemyDefender && EnemyDefender != CurrentEnemy)
		//	return;
		CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if(CurrentHUD && CurrentHUD->CombatOverlay)
		{
			CurrentHUD->CombatOverlay->SetNewEnemy(CurrentEnemy);
		}
		int32 Index = EnemyParty.Find(CurrentEnemy);
		for (int i = Index + 1; i < Index + CurrentSkill->NumberOfTargets; i++)
		{
			if (i >= EnemyParty.Num()) return;
			EnemyParty[i]->SetArrowToColor();
			EnemyParty[i]->ShowArrow();
		}
	}
}

void UFight::HandleOnCharacterEndHover(ACombatCharacter* HoveredCharacter)
{
	if (HoveredCharacter == nullptr || CurrentSkill == nullptr) return;
	if (CurrentSkill->NumberOfTargets <= 1) return;
	if (HoveredCharacter->IsA(AAlly::StaticClass()))
	{
		int32 Index = AlliedParty.Find(Cast<AAlly>(HoveredCharacter));
		for (int i = Index + 1; i < Index + CurrentSkill->NumberOfTargets; i++)
		{
			if (i >= AlliedParty.Num()) return;
			AlliedParty[i]->HideArrow();
		}
	}
	else
	{
		int32 Index = EnemyParty.Find(Cast<AEnemy>(HoveredCharacter));
		for (int i = Index + 1; i < Index + CurrentSkill->NumberOfTargets; i++)
		{
			if (i >= EnemyParty.Num()) return;
			EnemyParty[i]->HideArrow();
		}
	}
}

void UFight::HandleOnCharacterClicked(ACombatCharacter* ClickedCharacter)
{
	Targets.Empty();
	ClearEnemyArrows();
	if (ClickedCharacter == nullptr || CurrentSkill==nullptr) return;
	if (ClickedCharacter->IsA(AAlly::StaticClass()))
	{
		int32 Index = AlliedParty.Find(Cast<AAlly>(ClickedCharacter));
		for (int i = Index; i < Index + CurrentSkill->NumberOfTargets; i++)
		{
			if (i >= AlliedParty.Num()) break;
			Targets.Add(AlliedParty[i]);
		}
	}
	else
	{
		AEnemy* CurrentEnemy = Cast<AEnemy>(ClickedCharacter);
		AEnemy* EnemyDefender = GetDefenderEnemy();
		if (CurrentEnemy && EnemyDefender && EnemyDefender != CurrentEnemy)
			return;
		int32 Index = EnemyParty.Find(CurrentEnemy);
		for (int i = Index; i < Index + CurrentSkill->NumberOfTargets; i++)
		{
			if (i >= EnemyParty.Num()) break;
			Targets.Add(EnemyParty[i]);
		}
	}
	if (CurrentSkill && !Targets.IsEmpty())
	{
		if (CurrentSkill->bMelee)
		{
			TriggerMelee(CurrentCharacter);
			DisableMouseEvents();
		}
		else
		{
			if(CurrentCharacter && Encounter)
			{
				Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
				UseSkill();
				EndTurn();
			}
		}
	}
}

void UFight::HandleOnTargetReached(ACombatCharacter* Character)
{
	if (Character && Character == CurrentCharacter)
	{
		UseSkillMelee();
	}
}

void UFight::HandleOnLocationReached(ACombatCharacter* Character)
{
	if (Character == nullptr || Character != CurrentCharacter || !Encounter) return;
	Character->Flip(false);
	if (Character->IsA(AAlly::StaticClass()))
	{
		Character->SetActorRotation(FRotator::ZeroRotator);
	}
	else
	{
		Character->SetActorRotation(FRotator(0.f, 180.f, 0.f));
	}
	Character->SetActorLocation(Character->TargetLocation);

	CurrentCharacter->GetWorldTimerManager().ClearTimer(CurrentCharacter->TurnTimer);
	Encounter->GetWorldTimerManager().ClearTimer(CurrentCharacter->TurnTimer);
	EndTurn();
}

void UFight::HandleOnCharacterDeath(ACombatCharacter* DeadCharacter)
{
	if (DeadCharacter == nullptr) return;
	TurnQueue.Remove(DeadCharacter);
	if (DeadCharacter->IsA(AAlly::StaticClass()))
	{
		if (AllAlliesDead())
		{
			Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
			Encounter->GetWorldTimerManager().SetTimer(EndFightTimer, Encounter, &AFightEncounter::EndFightFailure, 2.f);
		}
	}
	if (DeadCharacter->IsA(AEnemy::StaticClass()))
	{
		Encounter->GainedExp += DeadCharacter->GetExperience();
		EnemyParty.Remove(Cast<AEnemy>(DeadCharacter));
		if (AllEnemiesDead())
		{
			Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
			Encounter->GetWorldTimerManager().SetTimer(EndFightTimer, Encounter, &AFightEncounter::EndFightSuccess, 1.5f);
		}
		CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (CurrentHUD && CurrentHUD->CombatOverlay)
		{
			CurrentHUD->CombatOverlay->SetNewEnemy(nullptr);
		}
		DeadCharacter->SetLifeSpan(2.f);
	}
	TurnQueue.Empty();
	BuildTurnOrder();
}

void UFight::UseSkill()
{
	if (CurrentCharacter == nullptr || CurrentSkill == nullptr || Targets.IsEmpty()) return;
	CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD)
	{
		CurrentHUD->ShowInfoText(CurrentSkill->SkillName);
	}
	if (CurrentSkill->IsA(USummonSkill::StaticClass()))
	{
		USummonSkill* SummonSkill = Cast<USummonSkill>(CurrentSkill);
		if (SummonSkill)
		{
			UseSummonSkill(SummonSkill);
		}
	}
	CurrentSkill->UseSkill(CurrentCharacter, Targets);
}

void UFight::UseSummonSkill(USummonSkill* Skill)
{
	int32 EnemyPartySize = EnemyParty.Num();
	if (EnemyPartySize >= 4 || Encounter==nullptr)
	{
		return;
	}
	FVector DesiredLocation = EnemyParty[EnemyPartySize - 1]->GetActorLocation();
	Encounter->OffestEnemySpawnLocation(DesiredLocation);
	Encounter->SpawnEnemy(DesiredLocation, Skill->SummonedEnemyClasses);
}

void UFight::UseItem(UItem* Item)
{
	CurrentHUD = Cast<ADefaultHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (CurrentHUD && CurrentHUD->InventoryWidget)
	{
		CurrentHUD->InventoryWidget->UpdateInventorySlots();
		CurrentHUD->HideInventory();
	}
	AAlly* CurrentAlly = Cast<AAlly>(CurrentCharacter);
	if (Item == nullptr || CurrentCharacter == nullptr || CurrentAlly == nullptr) return;
	Item->UseItem(CurrentAlly);
	CurrentAlly->bCanUseItems = false;
}

void UFight::HandleOnCharacterEndTurn(ACombatCharacter* Character)
{
	if (Encounter)
	{
		Encounter->GetWorldTimerManager().ClearTimer(TurnTimer);
	}
	if (CurrentCharacter && Character == CurrentCharacter)
	{
		bTurnInProgress = false;
		CurrentCharacter->OnCharacterEndTurn.RemoveDynamic(this, &UFight::HandleOnCharacterEndTurn);
		CurrentCharacter->HideArrow();
		CurrentCharacter->GetWorldTimerManager().ClearTimer(CurrentCharacter->TurnTimer);
		StartTurn();
	}
}

void UFight::UseSkillMelee()

{
	UseSkill();
	CurrentCharacter->GetWorldTimerManager().SetTimer(CurrentCharacter->TurnTimer, this, &UFight::CharacterMoveToLocation, 1.f);
}

void UFight::CharacterMoveToLocation()
{
	CurrentCharacter->GetWorldTimerManager().ClearTimer(CurrentCharacter->TurnTimer);
	CurrentCharacter->bShouldMoveToLocation = true;
}

void UFight::ClearEnemyArrows()
{
	for (AEnemy* Enemy : EnemyParty)
	{
		Enemy->HideArrow();
	}
}

void UFight::EnableMouseEvents()
{
	if (Controller)
	{

		Controller->bEnableClickEvents = true;
		Controller->bEnableMouseOverEvents = true;
	}
}

void UFight::DisableMouseEvents()
{
	if (Controller)
	{
		Controller->bEnableClickEvents = false;
		Controller->bEnableMouseOverEvents = false;
	}
}

inline bool UFight::IsFaster(ACombatCharacter* A, ACombatCharacter* B)
{
	if (A == nullptr || B == nullptr) return false;
	if (A->Attributes == nullptr || B->Attributes == nullptr) return false;
	uint8* SpeedA = A->Attributes->Stats.Find(TEXT("Szybko\u015b\u0107"));
	uint8* SpeedB = B->Attributes->Stats.Find(TEXT("Szybko\u015b\u0107"));

	if (SpeedA == nullptr || SpeedB == nullptr) return false;

	uint8 SpeedAValue = *SpeedA;
	uint8 SpeedBValue = *SpeedB;

	if (A->StatModifiers.Contains(TEXT("Szybko\u015b\u0107")))
	{
		int16 SpeedModifierA = *A->StatModifiers.Find(TEXT("Szybko\u015b\u0107"));
		if (SpeedModifierA < 0)
		{
			SpeedModifierA = FMath::Clamp(SpeedModifierA, 0-SpeedAValue, 0);
		}
		SpeedAValue += SpeedModifierA;
	}
	if (B->StatModifiers.Contains(TEXT("Szybko\u015b\u0107")))
	{
		int16 SpeedModifierB = *B->StatModifiers.Find(TEXT("Szybko\u015b\u0107"));
		if (SpeedModifierB < 0)
		{
			SpeedModifierB = FMath::Clamp(SpeedModifierB, 0-SpeedBValue, 0);
		}
		SpeedBValue += SpeedModifierB;
	}
	return SpeedAValue >= SpeedBValue;
}

bool UFight::AllAlliesDead()
{
	int16 AlliesAlive = 0;
	for (AAlly* Ally : AlliedParty)
	{
		if (!Ally->ActorHasTag(FName("Dead")))
		{
			AlliesAlive++;
		}
	}
	if (AlliesAlive == 0)
	{
		return true;
	}
	return false;
}

bool UFight::AllEnemiesDead()
{
	int16 EnemiesAlive = 0;
	for (AEnemy* Enemy : EnemyParty)
	{
		if (!Enemy->ActorHasTag(FName("Dead")))
		{
			EnemiesAlive++;
		}
	}
	if (EnemiesAlive == 0 || EnemyParty.IsEmpty())
	{
		return true;
	}
	return false;
}
