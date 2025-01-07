#include "UI/CombatOverlay.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameMechanics/AttributeComponent.h"
#include "GameMechanics/Skill.h"
#include "UI/CharacterIcon.h"
#include "UI/SkillIcon.h"
#include "Characters/Ally.h"
#include "Characters/Enemy.h"

void UCombatOverlay::SetNewAlly(AAlly* NewAlly)
{
	CurrentAlly = NewAlly;

	if(CurrentAlly)
	{
		AllyName->SetText(FText::FromString(CurrentAlly->CharacterName));
	}
	else
	{
		AllyName->SetText(FText::FromString(TEXT("")));
	}
	AllyPortrait->SetNewCharacter(CurrentAlly);

	TArray<USkillIcon*> AllySkillIcons = GetAllySkillIconsAsArray();
	TArray<USkill*> AllySkills;
	if (CurrentAlly)
	{
		AllySkills = CurrentAlly->EquippedSkills;
		if (AllySkills.IsEmpty()) return;
	}
	for (int i = 0; i < 4; i++)
	{
		if (i < AllySkills.Num() && AllySkills[i])
		{
			AllySkillIcons[i]->SetNewSkill(AllySkills[i]);
			AllySkillIcons[i]->CreateSkillDetails();
		}
		else
		{
			AllySkillIcons[i]->SetNewSkill(nullptr);
		}
	}
	if (CurrentAlly && CurrentAlly->Ult)
	{
		if (CurrentAlly->Attributes->GetRage() != CurrentAlly->Attributes->GetMaxRage())
		{
			CurrentAlly->Ult->bUsable = false;
			//AllyUlt->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			CurrentAlly->Ult->bUsable = true;
			//AllyUlt->SetVisibility(ESlateVisibility::Visible);
		}
		AllyUlt->SetNewSkill(CurrentAlly->Ult);
		AllyUlt->CreateSkillDetails();
	}
	else
	{
		AllyUlt->SetNewSkill(nullptr);
	}
}

void UCombatOverlay::SetNewEnemy(AEnemy* NewEnemy)
{
	CurrentEnemy = NewEnemy;

	if(CurrentEnemy)
	{
		EnemyName->SetText(FText::FromString(CurrentEnemy->CharacterName));
	}
	else
	{
		EnemyName->SetText(FText::FromString(TEXT("")));
	}
	EnemyPortrait->SetNewCharacter(CurrentEnemy);

	TArray<USkillIcon*> EnemySkillIcons = GetEnemySkillIconsAsArray();
	TArray<USkill*> EnemySkills;
	if(NewEnemy)
	{
		EnemySkills = CurrentEnemy->EquippedSkills;
		if (EnemySkills.IsEmpty()) return;
	}
	for (int i = 0; i < 4; i++)
	{
		if (i < EnemySkills.Num() && EnemySkills[i])
		{
			EnemySkillIcons[i]->SetNewSkill(EnemySkills[i]);
			EnemySkillIcons[i]->CreateSkillDetails();
		}
		else
		{
			EnemySkillIcons[i]->SetNewSkill(nullptr);
		}
	}
}

void UCombatOverlay::EndTurnButtonClicked()
{
	OnEndTurnButtonClicked.Broadcast();
}

TArray<USkillIcon*> UCombatOverlay::GetAllySkillIconsAsArray()
{
	TArray<USkillIcon*> SkillIcons;
	SkillIcons.Add(AllySkill1);
	SkillIcons.Add(AllySkill2);
	SkillIcons.Add(AllySkill3);
	SkillIcons.Add(AllySkill4);
	return SkillIcons;
}

TArray<USkillIcon*> UCombatOverlay::GetEnemySkillIconsAsArray()
{
	TArray<USkillIcon*> SkillIcons;
	SkillIcons.Add(EnemySkill1);
	SkillIcons.Add(EnemySkill2);
	SkillIcons.Add(EnemySkill3);
	SkillIcons.Add(EnemySkill4);
	return SkillIcons;
}
