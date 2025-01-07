#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "GameMechanics/GameModes/DefaultGameInstance.h"
#include "GameMechanics/AttributeComponent.h"
#include "GameMechanics/CharacterSpawnPoint.h"
#include "GameMechanics/Quests/Quest.h"
#include "GameMechanics/Skill.h"
#include "GameMechanics/Effect.h"
#include "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DataStructures/CharacterStatData.h"
#include "DataStructures/SkillData.h"
#include "AIController.h"
#include "Items/Item.h"
#include "UI/DefaultHUD.h"

void ADefaultGameMode::BeginPlay()
{
	Super::BeginPlay();
	CurrentInstance = Cast<UDefaultGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!CurrentInstance) return;
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save00"), 0))
	{
		CurrentInstance->LoadGame();
	}
	else
	{
		if (MainCharacter == nullptr) return;
		InitializeCharacter(MainCharacter, FindAlly(MainCharacter));
		InitializeAllySkills(MainCharacter);
		AvailableAllies.AddUnique(MainCharacter);
		AlliedParty.AddUnique(MainCharacter);
		InitializeAlliesFromTable(MainCharacter);
	}
	//InitializeQuests();
}

FCharacterStatData* ADefaultGameMode::FindAlly(ACombatCharacter* Character)
{
	if (Character && !Character->CharacterName.IsEmpty() && Character->Attributes && AllyStatData)
	{
		FString NameWithoutSpaces = Character->CharacterName;
		NameWithoutSpaces.RemoveSpacesInline();
		FName CompositeKey(NameWithoutSpaces + TEXT("_") + FString::FromInt(Character->Attributes->GetLevel()));
		FCharacterStatData* CharacterData = AllyStatData->FindRow<FCharacterStatData>(CompositeKey, TEXT("Character Data Lookup"), true);
		return CharacterData;
	}
	return nullptr;
}

FCharacterStatData* ADefaultGameMode::FindEnemy(ACombatCharacter* Character)
{
	if (Character && !Character->CharacterName.IsEmpty() && Character->Attributes && EnemyStatData)
	{
		FString NameWithoutSpaces = Character->CharacterName;
		NameWithoutSpaces.RemoveSpacesInline();
		FName CompositeKey(NameWithoutSpaces + TEXT("_") + FString::FromInt(Character->Attributes->GetLevel()));
		FCharacterStatData* CharacterData = EnemyStatData->FindRow<FCharacterStatData>(CompositeKey, TEXT("Character Data Lookup"), true);
		return CharacterData;
	}
	return nullptr;
}

FSkillData* ADefaultGameMode::FindSkill(FString SkillName, int32 Level)
{
	FName CompositeKey(SkillName + TEXT("_") + FString::FromInt(Level));
	FSkillData* SkillRow = SkillData->FindRow<FSkillData>(CompositeKey, TEXT("Skill Data Lookup"), true);
	return SkillRow;
}

void ADefaultGameMode::InitializeAlliesFromTable(AMainCharacter* MainCharacter)
{
	UWorld* World = GetWorld();
	if (!World || !MainCharacter) return;
	TArray<FCharacterStatData*> CharacterRows;
	AllyStatData->GetAllRows<FCharacterStatData>(TEXT("Character Data Lookup"), CharacterRows);
	for (FCharacterStatData* CharacterRow : CharacterRows)
	{
		if (CharacterRow->CharacterName == MainCharacter->CharacterName) continue; //pomijamy Janusza i nieodblokowanych sojusznikow (rank -1 oznacza, ze go nie odblokowalismy; rank 0, ze nie w druzynie a rank 1-4 to druzyna)
		FVector SpawnLocation;
		FRotator SpawnRotation = MainCharacter->GetActorRotation();
		if (CharacterRow->Rank > 0)
		{
			SpawnLocation = MainCharacter->GetActorLocation();
			SpawnLocation.X -= (CharacterRow->Rank - 1) * 80.f; //jesli w druzynie to spawnujemy za Januszem
		}
		else
		{
			if(FreeSpawnPointIndex < AllySpawnPoints.Num())
			{
				SpawnLocation = AllySpawnPoints[FreeSpawnPointIndex]->GetActorLocation(); // jesli nie w druzynie to spawnujemy pod mapa, zeby miec dostep
				FreeSpawnPointIndex++;
			}
		}
		AAlly* NewAlly = World->SpawnActor<AAlly>(CharacterRow->CharacterClass, SpawnLocation, SpawnRotation);
		InitializeCharacter(NewAlly, CharacterRow);
		InitializeAllySkills(NewAlly);
		if (CharacterRow->Rank >= 0)
		{
			AvailableAllies.AddUnique(NewAlly);
		}
		if(CharacterRow->Rank>0)
		{
			AlliedParty.AddUnique(NewAlly);
		}
		NewAlly->Rank = CharacterRow->Rank;
		AAIController* NewAIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), SpawnLocation, SpawnRotation);
		NewAIController->Possess(NewAlly);
		NewAlly->AIController = NewAIController;
	}
}

void ADefaultGameMode::InitializeCharacter(ACombatCharacter* Character, FCharacterStatData* CharacterData)
{
	if (Character && CharacterData)
	{
		InitializeCharacterStats(Character, CharacterData);

		InitializeCharacterResistances(Character, CharacterData);

		SetCharacterHealthAndMana(Character, CharacterData);

		SetCharacterExp(Character, CharacterData);
	}
}

void ADefaultGameMode::InitializeAllySkills(AAlly* Character)
{
	if (Character && !Character->SkillNames.IsEmpty() && SkillData)
	{
		for (FString SkillName : Character->SkillNames)
		{
			for (int i = 1; i <= 5; i++)
			{
				FSkillData* SkillRow = FindSkill(SkillName, i);
				if (SkillRow == nullptr) continue;
				USkill* NewSkill = NewObject<USkill>(Character, SkillRow->SkillClass);
				NewSkill->Level = i;
				NewSkill->SkillName = SkillRow->SkillName;
				NewSkill->MinValue = SkillRow->MinValue;
				NewSkill->MaxValue = SkillRow->MaxValue;
				NewSkill->BaseAccuracy = SkillRow->BaseAccuracy;
				NewSkill->BaseCritRate = SkillRow->BaseCritRate;
				NewSkill->StunRate = SkillRow->StunRate;
				NewSkill->SelfHeal = SkillRow->SelfHeal;
				NewSkill->SelfDamage = SkillRow->SelfDamage;
				NewSkill->ManaCost = SkillRow->ManaCost;
				NewSkill->bUnlocked = SkillRow->bUnlocked;
				
				if(SkillRow->InflictedEffectClass)
				{
					UEffect* InflictedEffect = NewObject<UEffect>(NewSkill, SkillRow->InflictedEffectClass);
					InflictedEffect->SetValue(SkillRow->InflictedEffectValue);
					InflictedEffect->SetDuration(SkillRow->InflictedEffectDuration);
					NewSkill->SetInflictedEffect(InflictedEffect);
				}
				TArray<UEffect*> SelfEffects;
				if(SkillRow->SelfEffect1Class)
				{
					UEffect* SelfEffect = NewObject<UEffect>(NewSkill, SkillRow->SelfEffect1Class);
					SelfEffect->SetValue(SkillRow->SelfEffect1Value);
					SelfEffect->SetDuration(SkillRow->SelfEffect1Duration);
					SelfEffects.Add(SelfEffect);
				}
				if (SkillRow->SelfEffect2Class)
				{
					UEffect* SelfEffect = NewObject<UEffect>(NewSkill, SkillRow->SelfEffect2Class);
					SelfEffect->SetValue(SkillRow->SelfEffect2Value);
					SelfEffect->SetDuration(SkillRow->SelfEffect2Duration);
					SelfEffects.Add(SelfEffect);
				}
				if(!SelfEffects.IsEmpty())
				{
					NewSkill->SetSelfEffects(SelfEffects);
				}
				NewSkill->BuildDescription();
				Character->Skills.Add(NewSkill);
				if (SkillRow->bEquipped)
				{
					if (NewSkill->SkillType == ESkillType::EST_UltAttack || NewSkill->SkillType == ESkillType::EST_UltSupport)
					{
						Character->Ult = NewSkill;
					}
					else
					{
						Character->EquippedSkills.Add(NewSkill);
					}
				}
			}
		}
	}
}

void ADefaultGameMode::InitializeEnemySkills(ACombatCharacter* Character, int32 Level)
{
	if (Character && !Character->SkillNames.IsEmpty() && SkillData)
	{
		for (FString SkillName : Character->SkillNames)
		{
			int32 SkillLevel = Level % 2 == 0 ? (Level / 2) : (Level+1)/2;
			FSkillData* SkillRow = FindSkill(SkillName, SkillLevel);
			if (SkillRow == nullptr || SkillRow->SkillClass == nullptr) continue;
			USkill* NewSkill = NewObject<USkill>(Character, SkillRow->SkillClass);
			NewSkill->Level = SkillLevel;
			NewSkill->SkillName = SkillRow->SkillName;
			NewSkill->MinValue = SkillRow->MinValue;
			NewSkill->MaxValue = SkillRow->MaxValue;
			NewSkill->BaseAccuracy = SkillRow->BaseAccuracy;
			NewSkill->BaseCritRate = SkillRow->BaseCritRate;
			NewSkill->StunRate = SkillRow->StunRate;
			NewSkill->SelfHeal = NewSkill->SelfHeal;
			NewSkill->SelfDamage = SkillRow->SelfDamage;
			NewSkill->ManaCost = SkillRow->ManaCost;
			NewSkill->bUnlocked = SkillRow->bUnlocked;

			if (SkillRow->InflictedEffectClass)
			{
				UEffect* InflictedEffect = NewObject<UEffect>(NewSkill, SkillRow->InflictedEffectClass);
				InflictedEffect->SetValue(SkillRow->InflictedEffectValue);
				InflictedEffect->SetDuration(SkillRow->InflictedEffectDuration);
				NewSkill->SetInflictedEffect(InflictedEffect);
			}
			TArray<UEffect*> SelfEffects;
			if (SkillRow->SelfEffect1Class)
			{
				UEffect* SelfEffect = NewObject<UEffect>(NewSkill, SkillRow->SelfEffect1Class);
				SelfEffect->SetValue(SkillRow->SelfEffect1Value);
				SelfEffect->SetDuration(SkillRow->SelfEffect1Duration);
				SelfEffects.Add(SelfEffect);
			}
			if (SkillRow->SelfEffect2Class)
			{
				UEffect* SelfEffect = NewObject<UEffect>(NewSkill, SkillRow->SelfEffect2Class);
				SelfEffect->SetValue(SkillRow->SelfEffect2Value);
				SelfEffect->SetDuration(SkillRow->SelfEffect2Duration);
				SelfEffects.Add(SelfEffect);
			}
			if (!SelfEffects.IsEmpty())
			{
				NewSkill->SetSelfEffects(SelfEffects);
			}
			NewSkill->BuildDescription();
			Character->EquippedSkills.Add(NewSkill);
		}
	}
}

void ADefaultGameMode::InitializeQuests()
{
	TArray<AActor*> QuestActors;
	if(QuestBaseClass)
	{
		UGameplayStatics::GetAllActorsOfClass(this, QuestBaseClass, QuestActors);
		for (AActor* QuestActor : QuestActors)
		{
			AQuest* Quest = Cast<AQuest>(QuestActor);
			if (Quest)
			{
				Quests.AddUnique(Quest);
			}
		}
	}
}

void ADefaultGameMode::InitializeCharacterStats(ACombatCharacter* Character, FCharacterStatData* CharacterData)
{
	Character->Attributes->Stats.Add(TEXT("Witalno\u015b\u0107"), CharacterData->Vitality);
	Character->Attributes->Stats.Add(TEXT("Magia"), CharacterData->Magic);
	Character->Attributes->Stats.Add(TEXT("Zdolno\u015b\u0107 do Ludob\u00f3jstwa"), CharacterData->CapacityForGenocide);
	Character->Attributes->Stats.Add(TEXT("Bieg\u0142o\u015b\u0107 Lecznicza"), CharacterData->HealingProficiency);
	Character->Attributes->Stats.Add(TEXT("Obrona"), CharacterData->Defence);
	Character->Attributes->Stats.Add(TEXT("Narwano\u015b\u0107"), CharacterData->Speed);
	Character->Attributes->Stats.Add(TEXT("Unik"), CharacterData->Dodge);
	Character->Attributes->Stats.Add(TEXT("Szcz\u0119\u015bcie"), CharacterData->Luck);
	Character->Attributes->Stats.Add(TEXT("Celno\u015b\u0107"), CharacterData->Accuracy);
}

void ADefaultGameMode::InitializeCharacterResistances(ACombatCharacter* Character, FCharacterStatData* CharacterData)
{
	Character->Attributes->Resistances.Add(TEXT("Og\u0142uszenie"), CharacterData->StunResistance);
	Character->Attributes->Resistances.Add(TEXT("Ciemi\u0119\u017cenie"), CharacterData->OppressionResistance);
	Character->Attributes->Resistances.Add(TEXT("Smrud"), CharacterData->StinkResistance);
	Character->Attributes->Resistances.Add(TEXT("\u017byczenie \u015amierci"), CharacterData->DeathwishResistance);
	Character->Attributes->Resistances.Add(TEXT("Zamulenie"), CharacterData->TiredResistance);
	Character->Attributes->Resistances.Add(TEXT("Najebanie"), CharacterData->DrunkResistance);
	Character->Attributes->Resistances.Add(TEXT("Kompromitacja Cwela"), CharacterData->DisgraceResistance);
}

void ADefaultGameMode::SetCharacterHealthAndMana(ACombatCharacter* Character, FCharacterStatData* CharacterData)
{
	Character->Attributes->SetMaxHealth(10.f + (CharacterData->Vitality - 5.f));
	Character->Attributes->SetHealth(10.f + (CharacterData->Vitality - 5.f));
	Character->Attributes->SetMaxMana(10.f + (CharacterData->Magic - 5.f));
	Character->Attributes->SetMana(10.f + (CharacterData->Magic - 5.f));
}

void ADefaultGameMode::SetCharacterExp(ACombatCharacter* Character, FCharacterStatData* CharacterData)
{
	Character->SetExpToNextLevel(CharacterData->ExpToNextLevel);
	Character->Attributes->SetLevel(CharacterData->Level);
	Character->SetExperience(CharacterData->Experience);
	Character->Rank = CharacterData->Rank;
}

void ADefaultGameMode::AddToParty(AAlly* Ally, int32 Rank)
{
	if (Rank <= 0 || Rank>4) return;
	if(Ally)
	{
		AlliedParty.Insert(Ally, Rank - 1);
	}
}

TArray<UItem*> ADefaultGameMode::GetAllFoodItems()
{
	TArray<UItem*> FoodItems;
	for (UItem* Item : Inventory)
	{
		if (Item->ItemType == EItemType::EIT_Food)
		{
			FoodItems.Add(Item);
		}
	}
	return FoodItems;
}

void ADefaultGameMode::UpdateAlliesLocation()
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MainCharacter == nullptr) return;
	FVector Location = MainCharacter->GetActorLocation();
	for (int i = 1; i < AlliedParty.Num(); i++)
	{
		AlliedParty[i]->Rank = i + 1;
		Location.X -= 80.f;
		AlliedParty[i]->SetActorLocation(Location);
	}
}

void ADefaultGameMode::RemoveAlly(FString AllyName, bool bRemovePermanently)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (Controller == nullptr || Controller->GetHUD()==nullptr) return;
	ADefaultHUD* HUD = Cast<ADefaultHUD>(Controller->GetHUD());
	if (HUD == nullptr) return;
	for (AAlly* Ally : AvailableAllies)
	{
		if (Ally->CharacterName.Equals(AllyName))
		{
			if (AlliedParty.Contains(Ally))
			{
				AlliedParty.Remove(Ally);
			}
			if (bRemovePermanently)
			{
				AvailableAllies.Remove(Ally);
				if (FreeSpawnPointIndex < AllySpawnPoints.Num())
				{
					Ally->SetActorLocation(AllySpawnPoints[FreeSpawnPointIndex]->GetActorLocation());
				}
			}
			if (HUD)
			{
				HUD->UpdateOverlays();
			}
			break;
		}
	}
}

//void ADefaultGameMode::AddNewAlly(AAlly* NewAlly, int32 Rank)
//{
//	if (!NewAlly || Rank > 4) return;
//
//	// Remove existing ally at that rank
//	if (Rank - 1 < AlliedParty.Num())
//	{
//		if (AAlly* ExistingAlly = AlliedParty[Rank - 1])
//		{
//			ExistingAlly->Rank = 0;
//			AlliedParty.Remove(ExistingAlly);
//		}
//	}
//
//	// Add new ally
//	NewAlly->Rank = Rank;
//	AlliedParty.Insert(NewAlly, Rank - 1);
//
//	// Ensure party size limit
//	if (AlliedParty.Num() > 4)
//	{
//		AlliedParty.RemoveAt(4);
//	}
//
//	// Show HUD notification
//	if (auto CurrentHUD = Cast<ADefaultHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD()))
//	{
//		CurrentHUD->ShowInfoText(TEXT("Nowy cz\u0142onek dru\u017cyny: ") + NewAlly->CharacterName);
//		CurrentHUD->UpdateOverlays();
//	}
//	UpdateAlliesLocation();
//}

void ADefaultGameMode::AddNewAlly(AAlly* NewAlly, int32 Rank)
{
	if (NewAlly == nullptr || Rank>4) return;
	AvailableAllies.AddUnique(NewAlly);
	NewAlly->Rank = Rank;
	if (Rank - 1 > AlliedParty.Num())
	{
		AlliedParty.AddUnique(NewAlly);
		UpdateAlliesLocation();
		return;
	}
	AlliedParty[Rank - 1]->Rank = 0;
	TArray<AAlly*> TempAllies;
	for (int i = AlliedParty.Num()-1; i>=Rank-1; i--)
	{
		AAlly* OldAlly = AlliedParty[i];
		if (OldAlly && OldAlly!=NewAlly)
		{
			TempAllies.AddUnique(OldAlly);
			AlliedParty.Remove(OldAlly);
		}
	}
	AlliedParty.AddUnique(NewAlly);
	for (int i = TempAllies.Num() - 1; i >= 0; i--)
	{
		AAlly* Ally = TempAllies[i];
		AlliedParty.AddUnique(Ally);
	}
	if (AlliedParty.Num() == 5)
	{
		AlliedParty.RemoveAt(4);
	}
	if (auto CurrentHUD = Cast<ADefaultHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD()))
	{
		CurrentHUD->ShowInfoText(TEXT("Nowy cz\u0142onek dru\u017cyny: ") + NewAlly->CharacterName);
		CurrentHUD->UpdateOverlays();
	}
	UpdateAlliesLocation();
}
