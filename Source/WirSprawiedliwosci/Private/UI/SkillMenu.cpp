// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillMenu.h"
#include "UI/SkillIcon.h"
#include "Characters/Ally.h"
#include "Characters/MainCharacter.h"
#include "GameMechanics/Skill.h"
#include "Components/TextBlock.h"

void USkillMenu::SetNewAlly(AAlly* NewAlly)
{
	if (NewAlly == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("Bohater w menu: %s"), *NewAlly->CharacterName);
	CurrentAlly = NewAlly;
	UpdateSkillIcons();
	UpdateEquippedSkillIcons();
	UpdateSkillPointText();
}

void USkillMenu::UpdateSkillPointText()
{
	SkillPointsText->SetText(FText::FromString(TEXT("Punkty umiej\u0119tno\u015bci: ") + FString::FromInt(CurrentAlly->SkillPoints)));
}

void USkillMenu::UpdateSkillIcons()
{
	if (CurrentAlly == nullptr) return;
	TArray<USkillIcon*> SkillIcons = GetSkillIconsAsArray();
	for (int i = 0; i < SkillIcons.Num(); i++)
	{
		if (i < CurrentAlly->Skills.Num())
		{
			SkillIcons[i]->SetNewSkill(CurrentAlly->Skills[i]);
			SkillIcons[i]->CreateSkillDetails();
			SkillIcons[i]->OnSkillIconClicked.AddUniqueDynamic(this, &USkillMenu::HandleSkillIconClicked);
		}
		else
		{
			SkillIcons[i]->SetNewSkill(nullptr);
			SkillIcons[i]->CreateSkillDetails();
			SkillIcons[i]->OnSkillIconClicked.AddUniqueDynamic(this, &USkillMenu::HandleSkillIconClicked);
		}
	}
}

void USkillMenu::UpdateEquippedSkillIcons()
{
	if (CurrentAlly == nullptr) return;
	TArray<USkillIcon*> EquippedSkillIcons = GetEquippedSkillIconsAsArray();
	for (int i = 0; i < EquippedSkillIcons.Num(); i++)
	{
		if (i < CurrentAlly->EquippedSkills.Num())
		{
			EquippedSkillIcons[i]->SetNewSkill(CurrentAlly->EquippedSkills[i]);
			EquippedSkillIcons[i]->CreateSkillDetails();
		}
		else
		{
			EquippedSkillIcons[i]->SetNewSkill(nullptr);
			EquippedSkillIcons[i]->CreateSkillDetails();
		}
		EquippedSkillIcons[i]->OnSkillIconClicked.AddUniqueDynamic(this, &USkillMenu::HandleEquippedSkillIconClicked);
	}
	if (CurrentAlly->IsA(AMainCharacter::StaticClass()))
	{
		EquippedSkillUlt->SetNewSkill(CurrentAlly->Ult);
		EquippedSkillUlt->CreateSkillDetails();
		EquippedSkillUlt->OnSkillIconClicked.AddUniqueDynamic(this, &USkillMenu::HandleEquippedSkillIconClicked);
	}
	else
	{
		EquippedSkillUlt->SetNewSkill(nullptr);
	}
}

void USkillMenu::HandleSkillIconClicked(USkill* ClickedSkill)
{
	if (ClickedSkill == nullptr || CurrentAlly == nullptr) return;
	if (ClickedSkill->bUnlocked)
	{
		CurrentSkill = ClickedSkill;
	}
	else if (CurrentAlly->SkillPoints >= ClickedSkill->Level)
	{
		if(ClickedSkill->Level == 1 || CurrentAlly->Skills[CurrentAlly->Skills.Find(ClickedSkill) - 1]->bUnlocked)
		{
			CurrentAlly->SkillPoints -= ClickedSkill->Level;
			ClickedSkill->bUnlocked = true;
			CurrentSkill = ClickedSkill;
		}
	}
	UpdateSkillIcons();
	UpdateSkillPointText();
	UE_LOG(LogTemp, Log, TEXT("Skill clicked: %s, Level: %d, Unlocked: %s"), 
		*ClickedSkill->SkillName, ClickedSkill->Level, ClickedSkill->bUnlocked ? TEXT("true") : TEXT("false"));
}

void USkillMenu::HandleEquippedSkillIconClicked(USkill* ClickedSkill)
{
	if (CurrentAlly == nullptr) return;
	if (CurrentSkill == nullptr || !CurrentSkill->bUnlocked) return;
	if (ClickedSkill == nullptr)
	{
		if (CurrentSkill->SkillType == ESkillType::EST_UltSupport || CurrentSkill->SkillType == ESkillType::EST_UltAttack)
			CurrentAlly->Ult = CurrentSkill;
		else
			CurrentAlly->EquippedSkills.Add(CurrentSkill);
	}
	if (ClickedSkill && ClickedSkill==CurrentSkill) return;
	if(ClickedSkill && CurrentSkill)
	{
		if (CurrentSkill->SkillType == ESkillType::EST_UltSupport || CurrentSkill->SkillType == ESkillType::EST_UltAttack)
		{
			if (ClickedSkill->SkillType != ESkillType::EST_UltSupport && ClickedSkill->SkillType != ESkillType::EST_UltAttack) return;
			CurrentAlly->Ult = CurrentSkill;
		}
		if (CurrentSkill->SkillType != ESkillType::EST_UltSupport && CurrentSkill->SkillType != ESkillType::EST_UltAttack)
		{
			if (ClickedSkill->SkillType == ESkillType::EST_UltSupport || ClickedSkill->SkillType == ESkillType::EST_UltAttack) return;
			int32 Index = CurrentAlly->EquippedSkills.Find(ClickedSkill);
			CurrentAlly->EquippedSkills[Index] = CurrentSkill;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Equipped skill: %s, Level: %d, Unlocked: %s"),
		*CurrentSkill->SkillName, CurrentSkill->Level, CurrentSkill->bUnlocked ? TEXT("true") : TEXT("false"));
	CurrentSkill = nullptr;
	UpdateEquippedSkillIcons();
}

TArray<USkillIcon*> USkillMenu::GetSkillIconsAsArray()
{
	TArray<USkillIcon*> SkillIcons;
	SkillIcons.Add(SkillIcon1Lvl1);
	SkillIcons.Add(SkillIcon1Lvl2);
	SkillIcons.Add(SkillIcon1Lvl3);
	SkillIcons.Add(SkillIcon1Lvl4);
	SkillIcons.Add(SkillIcon1Lvl5);
	SkillIcons.Add(SkillIcon2Lvl1);
	SkillIcons.Add(SkillIcon2Lvl2);
	SkillIcons.Add(SkillIcon2Lvl3);
	SkillIcons.Add(SkillIcon2Lvl4);
	SkillIcons.Add(SkillIcon2Lvl5);
	SkillIcons.Add(SkillIcon3Lvl1);
	SkillIcons.Add(SkillIcon3Lvl2);
	SkillIcons.Add(SkillIcon3Lvl3);
	SkillIcons.Add(SkillIcon3Lvl4);
	SkillIcons.Add(SkillIcon3Lvl5);
	SkillIcons.Add(SkillIcon4Lvl1);
	SkillIcons.Add(SkillIcon4Lvl2);
	SkillIcons.Add(SkillIcon4Lvl3);
	SkillIcons.Add(SkillIcon4Lvl4);
	SkillIcons.Add(SkillIcon4Lvl5);
	SkillIcons.Add(SkillIcon5Lvl1);
	SkillIcons.Add(SkillIcon5Lvl2);
	SkillIcons.Add(SkillIcon5Lvl3);
	SkillIcons.Add(SkillIcon5Lvl4);
	SkillIcons.Add(SkillIcon5Lvl5);
	SkillIcons.Add(SkillIcon6Lvl1);
	SkillIcons.Add(SkillIcon6Lvl2);
	SkillIcons.Add(SkillIcon6Lvl3);
	SkillIcons.Add(SkillIcon6Lvl4);
	SkillIcons.Add(SkillIcon6Lvl5);
	SkillIcons.Add(SkillIcon7Lvl1);
	SkillIcons.Add(SkillIcon7Lvl2);
	SkillIcons.Add(SkillIcon7Lvl3);
	SkillIcons.Add(SkillIcon7Lvl4);
	SkillIcons.Add(SkillIcon7Lvl5);
	SkillIcons.Add(SkillIcon8Lvl1);
	SkillIcons.Add(SkillIcon8Lvl2);
	SkillIcons.Add(SkillIcon8Lvl3);
	SkillIcons.Add(SkillIcon8Lvl4);
	SkillIcons.Add(SkillIcon8Lvl5);
	SkillIcons.Add(SkillIcon9Lvl1);
	SkillIcons.Add(SkillIcon9Lvl2);
	SkillIcons.Add(SkillIcon9Lvl3);
	SkillIcons.Add(SkillIcon9Lvl4);
	SkillIcons.Add(SkillIcon9Lvl5);
	return SkillIcons;
}

TArray<USkillIcon*> USkillMenu::GetEquippedSkillIconsAsArray()
{
	TArray<USkillIcon*> EquippedSkillIcons;
	EquippedSkillIcons.Add(EquippedSkill1);
	EquippedSkillIcons.Add(EquippedSkill2);
	EquippedSkillIcons.Add(EquippedSkill3);
	EquippedSkillIcons.Add(EquippedSkill4);
	return EquippedSkillIcons;
}
