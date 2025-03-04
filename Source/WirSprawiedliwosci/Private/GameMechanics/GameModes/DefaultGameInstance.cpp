// Po kim ty taki chitry na prawa autorskie?!

#include "GameMechanics/GameModes/DefaultGameInstance.h"
#include "GameMechanics/GameModes/DefaultGameMode.h"
#include "GameMechanics/SaveGame/RPGSaveGame.h"
#include "GameMechanics/AttributeComponent.h"
#include "GameMechanics/CutsceneTrigger.h"
#include "GameMechanics/Skill.h"
#include "GameMechanics/Effect.h"
#include "GameMechanics/Combat/FightEncounter.h"
#include "GameMechanics/Quests/Quest.h"
#include "GameMechanics/Quests/QuestObjective.h"
#include "GameMechanics/Quests/ObjectiveCollection.h"
#include "Items/Item.h"
#include "Characters/Ally.h"
#include "Characters/MainCharacter.h"
#include "Characters/NPC.h"
#include "Environment/Building.h"
#include "Environment/InteractibleObject.h"
#include "DataStructures/AllyData.h"
#include "DataStructures/EffectData.h"
#include "DataStructures/CharacterStatData.h"
#include "Blueprint/UserWidget.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UDefaultGameInstance::Init()
{
	Super::Init();
}

void UDefaultGameInstance::SaveGame()
{
	ShowSaveIcon();
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("Save00"), 0) || SavedGame == nullptr)
	{
		SavedGame = Cast<URPGSaveGame>(UGameplayStatics::CreateSaveGameObject(URPGSaveGame::StaticClass()));
	}
	SaveParty();
	SaveWorld();
	SaveQuests();
	FAsyncSaveGameToSlotDelegate SaveGameDelegate;
	SaveGameDelegate.BindUObject(this, &UDefaultGameInstance::OnAsyncSaveFinished);
	UGameplayStatics::AsyncSaveGameToSlot(SavedGame, TEXT("Save00"), 0,
		SaveGameDelegate);
}

void UDefaultGameInstance::LoadGame()
{
	if (GetWorld() == nullptr || !UGameplayStatics::DoesSaveGameExist(TEXT("Save00"), 0)) return;
	SavedGame = Cast<URPGSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Save00"), 0));
	if (SavedGame == nullptr) return;
	LoadQuests();
	LoadWorld();
	LoadParty();
}

void UDefaultGameInstance::SaveParty()
{
	if (GetWorld() == nullptr || SavedGame == nullptr) return;
	TArray<AActor*> AllAllies;
	SavedGame->AlliesData.Empty();
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode == nullptr) return;
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(GameMode);
	if (CurrentGameMode == nullptr) return;
	for (AAlly* Ally : CurrentGameMode->AlliedParty)
	{
		SaveAlly(Ally);
	}
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAlly::StaticClass(), AllAllies);
	for (AActor* Actor : AllAllies)
	{
		AAlly* Ally = Cast<AAlly>(Actor);
		if(Ally && !CurrentGameMode->AlliedParty.Contains(Ally))
		{
			SaveAlly(Ally);
		}
	}
	SaveInventory();
}

void UDefaultGameInstance::SaveWorld()
{
	SaveNPCs();
	SaveBuildings();
	SaveInteractibles();
	SaveCutscenes();
	SaveFights();

	if (GetWorld() == nullptr || SavedGame == nullptr) return;
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode == nullptr) return;
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(GameMode);
	if (CurrentGameMode == nullptr) return;
	SavedGame->CurrentMode = CurrentGameMode->CurrentMode;

	SavedGame->KilledNPCs = KilledNPCs;
	SavedGame->Choices = Choices;
}

void UDefaultGameInstance::SaveQuests()
{
	if (GetWorld() == nullptr || SavedGame == nullptr) return;
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode == nullptr) return;
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(GameMode);
	if (CurrentGameMode == nullptr) return;
	SavedGame->ActiveQuestsData.Empty();
	for (AQuest* Quest : CurrentGameMode->Quests)
	{
		SaveQuest(Quest);
	}
}

void UDefaultGameInstance::LoadParty()
{
	LoadAllies();
	LoadInventory();
}

void UDefaultGameInstance::LoadWorld()
{
	if (GetWorld() == nullptr) return;
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode == nullptr) return;
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(GameMode);
	if (CurrentGameMode == nullptr) return;
	if (SavedGame == nullptr) return;
	CurrentGameMode->CurrentMode = SavedGame->CurrentMode;
	LoadNPCs();
	LoadBuildings();
	LoadInteractibles();
	LoadCutscenes();
	LoadFights();
	KilledNPCs = SavedGame->KilledNPCs;
	Choices = SavedGame->Choices;
}

void UDefaultGameInstance::LoadQuests()
{
	if (SavedGame == nullptr) return;
	for (FQuestData QuestData : SavedGame->ActiveQuestsData)
	{
		AActor* QuestActor = FindActorByName(QuestData.Name, AQuest::StaticClass());
		if (QuestActor)
		{
			AQuest* Quest = Cast<AQuest>(QuestActor);
			//Quest->SetRootObjectiveCollection(Quest->InitializeRoot());
			//Quest->InitializeObjectives();
			if(Quest) LoadQuest(Quest, QuestData);
		}
	}
}

void UDefaultGameInstance::SaveAlly(AAlly* Ally)
{
	FAllyData AllyData;
	if (Ally == nullptr || Ally->Attributes == nullptr) return;
	AllyData.Health = Ally->Attributes->GetHealth();
	AllyData.Mana = Ally->Attributes->GetMana();
	AllyData.Rage = Ally->Attributes->GetRage();
	AllyData.Location = Ally->GetActorLocation();
	AllyData.StatData = SaveAllyAttributes(Ally);
	AllyData.Skills = SaveAllySkills(Ally);
	AllyData.AppliedEffects = SaveAllyEffects(Ally);
	SavedGame->AlliesData.Add(AllyData);
}

FCharacterStatData UDefaultGameInstance::SaveAllyAttributes(AAlly* Ally)
{
	FCharacterStatData Stats;
	if (Ally == nullptr || Ally->Attributes == nullptr 
		|| Ally->Attributes->Stats.IsEmpty() || Ally->Attributes->Resistances.IsEmpty()) return Stats;
	Stats.CharacterClass = Ally->GetClass();
	Stats.CharacterName = Ally->CharacterName;
	Stats.Level = Ally->Attributes->GetLevel();

	//staty
	Stats.Vitality = *Ally->Attributes->Stats.Find(TEXT("Witalno\u015b\u0107"));
	Stats.Magic = *Ally->Attributes->Stats.Find(TEXT("Magia"));
	Stats.CapacityForGenocide = *Ally->Attributes->Stats.Find(TEXT("Zdolno\u015b\u0107 do Ludob\u00f3jstwa"));
	Stats.HealingProficiency = *Ally->Attributes->Stats.Find(TEXT("Bieg\u0142o\u015b\u0107 Lecznicza"));
	Stats.Defence = *Ally->Attributes->Stats.Find(TEXT("Obrona"));
	Stats.Speed = *Ally->Attributes->Stats.Find(TEXT("Narwano\u015b\u0107"));
	Stats.Dodge = *Ally->Attributes->Stats.Find(TEXT("Unik"));
	Stats.Luck = *Ally->Attributes->Stats.Find(TEXT("Szcz\u0119\u015bcie"));
	Stats.Accuracy = *Ally->Attributes->Stats.Find(TEXT("Celno\u015b\u0107"));

	//odpornosci
	Stats.StunResistance = *Ally->Attributes->Resistances.Find(TEXT("Og\u0142uszenie"));
	Stats.OppressionResistance = *Ally->Attributes->Resistances.Find(TEXT("Ciemi\u0119\u017cenie"));
	Stats.StinkResistance = *Ally->Attributes->Resistances.Find(TEXT("Smrud"));
	Stats.DeathwishResistance = *Ally->Attributes->Resistances.Find(TEXT("\u017byczenie \u015amierci"));
	Stats.TiredResistance = *Ally->Attributes->Resistances.Find(TEXT("Zamulenie"));
	Stats.DrunkResistance = *Ally->Attributes->Resistances.Find(TEXT("Najebanie"));
	Stats.DisgraceResistance = *Ally->Attributes->Resistances.Find(TEXT("Kompromitacja Cwela"));

	//exp
	Stats.Experience = Ally->GetExperience();
	Stats.ExpToNextLevel = Ally->GetExpToNextLevel();
	Stats.SkillPoints = Ally->SkillPoints;
	Stats.AttributePoints = Ally->AttributePoints;
	Stats.Rank = Ally->Rank;

	return Stats;
}

TArray<FSkillData> UDefaultGameInstance::SaveAllySkills(AAlly* Ally)
{
	TArray<FSkillData> SkillsData;
	for (USkill* Skill : Ally->Skills)
	{
		FSkillData SkillData;
		SkillData.SkillClass = Skill->GetClass();
		SkillData.SkillName = Skill->SkillName;
		SkillData.Level = Skill->Level;
		SkillData.MinValue = Skill->MinValue;
		SkillData.MaxValue = Skill->MaxValue;
		SkillData.BaseAccuracy = Skill->BaseAccuracy;
		SkillData.BaseCritRate = Skill->BaseCritRate;
		SkillData.StunRate = Skill->StunRate;
		SkillData.SelfDamage = Skill->SelfDamage;
		SkillData.SelfHeal = Skill->SelfHeal;
		SkillData.ManaCost = Skill->ManaCost;

		//efekty
		if (Skill->GetInflictedEffect())
		{
			SkillData.InflictedEffectClass = Skill->GetInflictedEffect()->GetClass();
			SkillData.InflictedEffectDuration = Skill->GetInflictedEffect()->GetDuration();
			SkillData.InflictedEffectValue = Skill->GetInflictedEffect()->GetValue();
		}

		if (!Skill->GetSelfEffects().IsEmpty() && Skill->GetSelfEffects()[0])
		{
			SkillData.SelfEffect1Class = Skill->GetSelfEffects()[0]->GetClass();
			SkillData.SelfEffect1Duration = Skill->GetSelfEffects()[0]->GetDuration();
			SkillData.SelfEffect1Value = Skill->GetSelfEffects()[0]->GetValue();
		}

		if (Skill->GetSelfEffects().Num()>1 && Skill->GetSelfEffects()[1])
		{
			SkillData.SelfEffect2Class = Skill->GetSelfEffects()[1]->GetClass();
			SkillData.SelfEffect2Duration = Skill->GetSelfEffects()[1]->GetDuration();
			SkillData.SelfEffect2Value = Skill->GetSelfEffects()[1]->GetValue();
		}

		SkillData.bUnlocked = Skill->bUnlocked;
		if (Ally->EquippedSkills.Contains(Skill) || (Ally->Ult && Skill == Ally->Ult))
		{
			SkillData.bEquipped = true;
		}
		else
		{
			SkillData.bEquipped = false;
		}
		SkillsData.Add(SkillData);
	}
	return SkillsData;
}

TArray<FEffectData> UDefaultGameInstance::SaveAllyEffects(AAlly* Ally)
{
	if (Ally == nullptr)
	{
		return TArray<FEffectData>();
	}
	if (Ally->GetEffects().IsEmpty())
	{
		return TArray<FEffectData>();
	}
	TArray<FEffectData> EffectsData;
	for (UEffect* Effect : Ally->GetEffects())
	{
		FEffectData EffectData;
		EffectData.EffectClass = Effect->GetClass();
		EffectData.Value = Effect->GetValue();
		EffectData.Duration = Effect->GetDuration();
		EffectsData.Add(EffectData);
	}
	return EffectsData;
}

void UDefaultGameInstance::LoadAllies()
{	
	UWorld* World = GetWorld();
	if (World == nullptr || SavedGame==nullptr) return;
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(World);
	if (GameMode == nullptr) return;
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(GameMode);
	if (CurrentGameMode == nullptr) return;
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	for (FAllyData AllyData : SavedGame->AlliesData)
	{
		if (AllyData.StatData.CharacterClass->IsChildOf(AMainCharacter::StaticClass()))
		{
			if (MainCharacter && MainCharacter->Attributes)
			{
				MainCharacter->SetActorLocation(AllyData.Location);
				CurrentGameMode->InitializeCharacter(MainCharacter, &AllyData.StatData);
				LoadAllySkills(MainCharacter, AllyData);
				LoadAllyEffects(MainCharacter, AllyData);
				MainCharacter->Attributes->SetHealth(AllyData.Health);
				MainCharacter->Attributes->SetMana(AllyData.Mana);
				MainCharacter->Attributes->SetRage(AllyData.Rage);
				MainCharacter->AttributePoints = AllyData.StatData.AttributePoints;
				MainCharacter->SkillPoints = AllyData.StatData.SkillPoints;
				MainCharacter->Rank = AllyData.StatData.Rank;
				CurrentGameMode->AvailableAllies.AddUnique(MainCharacter);
				CurrentGameMode->AlliedParty.AddUnique(MainCharacter);
			}
		}
		else
		{
			AAlly* NewAlly = World->SpawnActor<AAlly>(AllyData.StatData.CharacterClass, AllyData.Location, MainCharacter->GetActorRotation());
			if(NewAlly && NewAlly->Attributes)
			{
				CurrentGameMode->InitializeCharacter(NewAlly, &AllyData.StatData);
				LoadAllySkills(NewAlly, AllyData);
				LoadAllyEffects(NewAlly, AllyData);
				NewAlly->Attributes->SetHealth(AllyData.Health);
				NewAlly->Attributes->SetMana(AllyData.Mana);
				NewAlly->Attributes->SetRage(AllyData.Rage);
				NewAlly->AttributePoints = AllyData.StatData.AttributePoints;
				NewAlly->SkillPoints = AllyData.StatData.SkillPoints;
				NewAlly->Rank = AllyData.StatData.Rank;
				if (NewAlly->Rank >= 0)
				{
					CurrentGameMode->AvailableAllies.AddUnique(NewAlly);
				}
				if (NewAlly->Rank > 0)
				{
					CurrentGameMode->AlliedParty.AddUnique(NewAlly);
				}
				AAIController* NewAIController = World->SpawnActor<AAIController>(AAIController::StaticClass(), AllyData.Location, FRotator::ZeroRotator);
				NewAIController->Possess(NewAlly);
				NewAlly->AIController = NewAIController;
			}
		}
	}
}

void UDefaultGameInstance::LoadAllySkills(AAlly* Ally, FAllyData AllyData)
{
	for (FSkillData SkillData : AllyData.Skills)
	{
		USkill* NewSkill = NewObject<USkill>(Ally, SkillData.SkillClass);
		NewSkill->Level = SkillData.Level;
		NewSkill->SkillName = SkillData.SkillName;
		NewSkill->MinValue = SkillData.MinValue;
		NewSkill->MaxValue = SkillData.MaxValue;
		NewSkill->BaseAccuracy = SkillData.BaseAccuracy;
		NewSkill->BaseCritRate = SkillData.BaseCritRate;
		NewSkill->StunRate = SkillData.StunRate;
		NewSkill->SelfHeal = SkillData.SelfHeal;
		NewSkill->SelfDamage = SkillData.SelfDamage;
		NewSkill->ManaCost = SkillData.ManaCost;
		NewSkill->bUnlocked = SkillData.bUnlocked;

		if (SkillData.InflictedEffectClass)
		{
			UEffect* InflictedEffect = NewObject<UEffect>(NewSkill, SkillData.InflictedEffectClass);
			InflictedEffect->SetValue(SkillData.InflictedEffectValue);
			InflictedEffect->SetDuration(SkillData.InflictedEffectDuration);
			NewSkill->SetInflictedEffect(InflictedEffect);
		}
		TArray<UEffect*> SelfEffects;
		if (SkillData.SelfEffect1Class)
		{
			UEffect* SelfEffect = NewObject<UEffect>(NewSkill, SkillData.SelfEffect1Class);
			SelfEffect->SetValue(SkillData.SelfEffect1Value);
			SelfEffect->SetDuration(SkillData.SelfEffect1Duration);
			SelfEffects.Add(SelfEffect);
		}
		if (SkillData.SelfEffect2Class)
		{
			UEffect* SelfEffect = NewObject<UEffect>(NewSkill, SkillData.SelfEffect2Class);
			SelfEffect->SetValue(SkillData.SelfEffect2Value);
			SelfEffect->SetDuration(SkillData.SelfEffect2Duration);
			SelfEffects.Add(SelfEffect);
		}
		if (!SelfEffects.IsEmpty())
		{
			NewSkill->SetSelfEffects(SelfEffects);
		}
		NewSkill->BuildDescription();
		Ally->Skills.Add(NewSkill);
		if (SkillData.bEquipped)
		{
			if (NewSkill->SkillType == ESkillType::EST_UltAttack || NewSkill->SkillType == ESkillType::EST_UltSupport)
			{
				Ally->Ult = NewSkill;
			}
			else
			{
				Ally->EquippedSkills.Add(NewSkill);
			}
		}
	}
}

void UDefaultGameInstance::LoadAllyEffects(AAlly* Ally, FAllyData AllyData)
{
	if (Ally == nullptr)
	{
		return;
	}
	if (AllyData.AppliedEffects.IsEmpty())
	{
		return;
	}
	for (FEffectData EffectData : AllyData.AppliedEffects)
	{
		UEffect* Effect = NewObject<UEffect>(Ally, EffectData.EffectClass);
		Effect->SetValue(EffectData.Value);
		Effect->SetDuration(EffectData.Duration);
		Ally->AddEffect(Effect);
	}
}

void UDefaultGameInstance::SaveInventory()
{
	if (GetWorld() == nullptr || SavedGame==nullptr) return;
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode == nullptr) return;
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(GameMode);
	if (CurrentGameMode == nullptr) return;
	SavedGame->Money = CurrentGameMode->Money;
	SavedGame->InventoryData.Empty();
	for (UItem* Item : CurrentGameMode->Inventory)
	{
		FItemData ItemData;
		ItemData.ItemClass = Item->GetClass();
		ItemData.Quantity = Item->Quantity;
		SavedGame->InventoryData.Add(ItemData);
	}
}

void UDefaultGameInstance::LoadInventory()
{
	if (GetWorld() == nullptr) return;
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode == nullptr) return;
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(GameMode);
	if (CurrentGameMode == nullptr || SavedGame==nullptr) return;
	CurrentGameMode->Money = SavedGame->Money;
	for (FItemData ItemData : SavedGame->InventoryData)
	{
		UItem* Item = NewObject<UItem>(CurrentGameMode, ItemData.ItemClass);
		Item->Quantity = ItemData.Quantity;
		CurrentGameMode->Inventory.AddUnique(Item);
	}
}

void UDefaultGameInstance::SaveNPCs()
{
	if (GetWorld() == nullptr || SavedGame == nullptr) return;
	SavedGame->NPCsData.Empty();
	TArray<AActor*> NPCActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPC::StaticClass(), NPCActors);
	for (AActor* NPCActor : NPCActors)
	{
		ANPC* NPC = Cast<ANPC>(NPCActor);
		if (NPC)
		{
			FNPCData NPCData;
			NPCData.Name = *NPC->GetName();
			NPCData.bIsAlive = NPC->bIsAlive;
			if (NPC->SpawnPoint)
			{
				NPCData.SpawnPointName = NPC->SpawnPoint->GetName();
			}
			SavedGame->NPCsData.Add(NPCData);
		}
	}
}

void UDefaultGameInstance::LoadNPCs()
{
	if (GetWorld() == nullptr || SavedGame == nullptr) return;
	for (FNPCData NPCData : SavedGame->NPCsData)
	{
		AActor* NPCActor = FindActorByName(NPCData.Name.ToString(), ANPC::StaticClass());
		if (NPCActor)
		{
			ANPC* NPC = Cast<ANPC>(NPCActor);
			if (NPC)
			{
				NPC->bIsAlive = NPCData.bIsAlive;
				if (NPCData.SpawnPointName.IsEmpty()) continue;
					
				AActor* SpawnPointActor = FindActorByName(NPCData.SpawnPointName, ATargetPoint::StaticClass());
				if (SpawnPointActor) NPC->SpawnPoint = Cast<ATargetPoint>(SpawnPointActor);
			}
		}
	}
}

void UDefaultGameInstance::SaveBuildings()
{
	if (GetWorld() == nullptr || SavedGame == nullptr) return;
	SavedGame->BuildingsData.Empty();
	TArray<AActor*> BuildingActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuilding::StaticClass(), BuildingActors);
	for (AActor* BuildingActor : BuildingActors)
	{
		ABuilding* Building = Cast<ABuilding>(BuildingActor);
		if(Building)
		{
			FBuildingData BuildingData;
			BuildingData.Name = *Building->GetName();
			BuildingData.bIsAccessible = Building->IsAccessible();
			SavedGame->BuildingsData.Add(BuildingData);
		}
	}
}

void UDefaultGameInstance::SaveInteractibles()
{
	if (GetWorld() == nullptr || SavedGame == nullptr) return;
	SavedGame->InteractibleObjsData.Empty();
	TArray<AActor*> InteractibleActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractibleObject::StaticClass(), InteractibleActors);
	for (AActor* InteractibleActor : InteractibleActors)
	{
		AInteractibleObject* InteractibleObj = Cast<AInteractibleObject>(InteractibleActor);
		if(InteractibleObj)
		{
			FInteractibleObjData InteractibleObjData;
			InteractibleObjData.Name = *InteractibleObj->GetName();
			InteractibleObjData.bInteracted = InteractibleObj->bInteracted;
			SavedGame->InteractibleObjsData.Add(InteractibleObjData);
		}
	}
}

void UDefaultGameInstance::SaveCutscenes()
{
	if (GetWorld() == nullptr || SavedGame == nullptr) return;
	SavedGame->CutsceneTriggersData.Empty();
	TArray<AActor*> CutsceneActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACutsceneTrigger::StaticClass(), CutsceneActors);
	for (AActor* CutsceneActor : CutsceneActors)
	{
		ACutsceneTrigger* CutsceneTrigger = Cast<ACutsceneTrigger>(CutsceneActor);
		if (CutsceneTrigger)
		{
			FCutsceneTriggerData CutsceneTriggerData;
			CutsceneTriggerData.Name = *CutsceneTrigger->GetName();
			CutsceneTriggerData.bAlreadySeen = CutsceneTrigger->bAlreadySeen;
			SavedGame->CutsceneTriggersData.Add(CutsceneTriggerData);
		}
	}
}

void UDefaultGameInstance::SaveFights()
{
	if (GetWorld() == nullptr || SavedGame == nullptr) return;
	SavedGame->FightEncountersData.Empty();
	TArray<AActor*> FightActors;
	for (AActor* FightActor : FightActors)
	{
		AFightEncounter* FightEncounter = Cast<AFightEncounter>(FightActor);
		if (FightEncounter)
		{
			FFightEncounterData FightEncounterData;
			FightEncounterData.Name = *FightEncounter->GetName();
			FightEncounterData.RespawnsLeft = FightEncounter->RespawnsLeft;
			SavedGame->FightEncountersData.Add(FightEncounterData);
		}
	}
}

void UDefaultGameInstance::LoadBuildings()
{
	if (GetWorld() == nullptr || SavedGame == nullptr) return;
	for (FBuildingData BuildingData : SavedGame->BuildingsData)
	{
		AActor* BuildingActor = FindActorByName(BuildingData.Name.ToString(), ABuilding::StaticClass());
		if (BuildingActor)
		{
			ABuilding* Building = Cast<ABuilding>(BuildingActor);
			if (Building) Building->SetIsAccessible(BuildingData.bIsAccessible);
		}
	}
}

void UDefaultGameInstance::LoadInteractibles()
{
	if (GetWorld() == nullptr || SavedGame == nullptr) return;
	for (FInteractibleObjData InteractibleObjData : SavedGame->InteractibleObjsData)
	{
		AActor* InteractibleActor = FindActorByName(InteractibleObjData.Name.ToString(), AInteractibleObject::StaticClass());
		if (InteractibleActor)
		{
			AInteractibleObject* InteractibleObj = Cast<AInteractibleObject>(InteractibleActor);
			if (InteractibleObj) InteractibleObj->bInteracted = InteractibleObjData.bInteracted;
		}
	}
}

void UDefaultGameInstance::LoadCutscenes()
{
	if (GetWorld() == nullptr || SavedGame == nullptr) return;
	for (FCutsceneTriggerData CutsceneTriggerData : SavedGame->CutsceneTriggersData)
	{
		AActor* CutsceneActor = FindActorByName(CutsceneTriggerData.Name.ToString(), ACutsceneTrigger::StaticClass());
		if (CutsceneActor)
		{
			ACutsceneTrigger* CutsceneTrigger = Cast<ACutsceneTrigger>(CutsceneActor);
			if (CutsceneTrigger) CutsceneTrigger->bAlreadySeen = CutsceneTriggerData.bAlreadySeen;
		}
	}
}

void UDefaultGameInstance::LoadFights()
{
	if (GetWorld() == nullptr || SavedGame == nullptr) return;
	for (FFightEncounterData FightEncounterData : SavedGame->FightEncountersData)
	{
		AActor* FightActor = FindActorByName(FightEncounterData.Name.ToString(), AFightEncounter::StaticClass());
		if (FightActor)
		{
			AFightEncounter* FightEncounter = Cast<AFightEncounter>(FightActor);
			if (FightEncounter) FightEncounter->RespawnsLeft = FightEncounterData.RespawnsLeft;
		}
	}
}

void UDefaultGameInstance::SaveQuest(AQuest* Quest)
{
	FQuestData QuestData;
	QuestData.Name = Quest->GetName();
	if (Quest->GetRootObjectiveCollection()->bCompleted)
	{
		QuestData.CompletedObjectives.Add(Quest->GetRootObjectiveCollection()->ObjectiveName);
	}
	for (UQuestObjective* Objective : Quest->GetRootObjectiveCollection()->Objectives)
	{
		if (Objective->bActive)
		{
			QuestData.ActiveObjectives.Add(Objective->ObjectiveName);
		}
		if (Objective->bCompleted)
		{
			QuestData.CompletedObjectives.Add(Objective->ObjectiveName);
		}
		UObjectiveCollection* ObjCollection = Cast<UObjectiveCollection>(Objective);
		if (ObjCollection)
		{
			for (UQuestObjective* Subobjective : ObjCollection->Objectives)
			{
				if (Subobjective->bActive)
				{
					QuestData.ActiveObjectives.Add(Subobjective->ObjectiveName);
				}
				if (Subobjective->bCompleted)
				{
					QuestData.CompletedObjectives.Add(Subobjective->ObjectiveName);
				}
			}
		}
	}
	SavedGame->ActiveQuestsData.Add(QuestData);
}

void UDefaultGameInstance::LoadQuest(AQuest* Quest, FQuestData QuestData)
{
	UObjectiveCollection* RootObjective = Quest->GetRootObjectiveCollection();
	RootObjective->bActive = true;
	if (QuestData.CompletedObjectives.Contains(RootObjective->ObjectiveName))
	{
		RootObjective->bCompleted = true;
	}
	else
	{
		RootObjective->bCompleted = false;
	}
	for (UQuestObjective* Objective : RootObjective->Objectives)
	{
		if (QuestData.ActiveObjectives.Contains(Objective->ObjectiveName))
		{
			Objective->ActivateObjective(false);
			Objective->OnObjectiveCompleted.AddDynamic(Quest->GetRootObjectiveCollection(), &UObjectiveCollection::HandleObjectiveCompleted);
		}
		else
		{
			Objective->bActive = false;
		}
		if (QuestData.CompletedObjectives.Contains(Objective->ObjectiveName))
		{
			Objective->bCompleted = true;
		}
		else
		{
			Objective->bCompleted = false;
		}
		UObjectiveCollection* ObjCollection = Cast<UObjectiveCollection>(Objective);
		if (ObjCollection)
		{
			for (UQuestObjective* Subobjective : ObjCollection->Objectives)
			{
				if (QuestData.ActiveObjectives.Contains(Subobjective->ObjectiveName))
				{
					Subobjective->ActivateObjective(false);
					Subobjective->OnObjectiveCompleted.AddDynamic(Quest->GetRootObjectiveCollection(), &UObjectiveCollection::HandleObjectiveCompleted);
				}
				else
				{
					Subobjective->bActive = false;
				}
				if (QuestData.CompletedObjectives.Contains(Subobjective->ObjectiveName))
				{
					Subobjective->bCompleted = true;
				}
				else
				{
					Subobjective->bCompleted = false;
				}
			}
		}
	}
	if (GetWorld() == nullptr) return;
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode == nullptr) return;
	ADefaultGameMode* CurrentGameMode = Cast<ADefaultGameMode>(GameMode);
	if (CurrentGameMode == nullptr) return;
	CurrentGameMode->Quests.AddUnique(Quest);
}

void UDefaultGameInstance::ShowSaveIcon()
{
	if (SaveIconWidgetClass == nullptr) return;
	if (SaveIconWidget == nullptr)
	{
		SaveIconWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), SaveIconWidgetClass, FName("SaveIcon"));
	}
	SaveIconWidget->AddToViewport();
}

void UDefaultGameInstance::HideSaveIcon()
{
	if (SaveIconWidget == nullptr) return;
	SaveIconWidget->RemoveFromViewport();
}

void UDefaultGameInstance::OnAsyncSaveFinished(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if(bSuccess)
	{
		GetWorld()->GetTimerManager().SetTimer(SaveIconTimer, this, &UDefaultGameInstance::HideSaveIcon, 2.0f, false);
	}
}

AActor* UDefaultGameInstance::FindActorByName(FString SearchedName, TSubclassOf<AActor> ActorClass)
{
	if (GetWorld() == nullptr) return nullptr;
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActorClass, Actors);
	for (AActor* Actor : Actors)
	{
		if (Actor->GetName().Equals(SearchedName))
		{
			return Actor;
		}
	}
	return nullptr;
}

void UDefaultGameInstance::AddChoice(FString NewChoice)
{
	if (NewChoice.IsEmpty()) return;
	Choices.AddUnique(NewChoice);
}

bool UDefaultGameInstance::WasChoiceMade(FString SearchedChoice)
{
	if (Choices.Contains(SearchedChoice)) return true;
	return false;
}

bool UDefaultGameInstance::WasNPCKilled(FString NPCName)
{
	if (KilledNPCs.Contains(NPCName)) return true;
	return false;
}

void UDefaultGameInstance::AddToKilledNPCs(FString NPCName)
{
	if (NPCName.IsEmpty()) return;
	KilledNPCs.AddUnique(NPCName);
}
