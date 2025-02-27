// Po kim ty taki chitry na prawa autorskie?!

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DefaultGameInstance.generated.h"

class URPGSaveGame;
class AAlly;
class AQuest;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API UDefaultGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UFUNCTION(BlueprintCallable)
	void SaveParty();

	UFUNCTION(BlueprintCallable)
	void SaveWorld();

	UFUNCTION(BlueprintCallable)
	void SaveQuests();

	UFUNCTION(BlueprintCallable)
	void LoadParty();

	UFUNCTION(BlueprintCallable)
	void LoadWorld();

	UFUNCTION(BlueprintCallable)
	void LoadQuests();

	UFUNCTION()
	void SaveAlly(AAlly* Ally);

	UFUNCTION()
	FCharacterStatData SaveAllyAttributes(AAlly* Ally);

	UFUNCTION()
	TArray<FSkillData> SaveAllySkills(AAlly* Ally);

	UFUNCTION()
	TArray<FEffectData> SaveAllyEffects(AAlly* Ally);

	UFUNCTION()
	void LoadAllies();

	UFUNCTION()
	void LoadAllySkills(AAlly* Ally, FAllyData AllyData);

	UFUNCTION()
	void LoadAllyEffects(AAlly* Ally, FAllyData AllyData);

	UFUNCTION()
	void SaveInventory();

	UFUNCTION()
	void LoadInventory();

	UFUNCTION()
	void SaveNPCs();

	UFUNCTION()
	void LoadNPCs();

	UFUNCTION()
	void SaveBuildings();

	UFUNCTION()
	void SaveInteractibles();

	UFUNCTION()
	void SaveCutscenes();

	UFUNCTION()
	void SaveFights();

	UFUNCTION()
	void LoadBuildings();

	UFUNCTION()
	void LoadInteractibles();

	UFUNCTION()
	void LoadCutscenes();

	UFUNCTION()
	void LoadFights();

	UFUNCTION()
	void SaveQuest(AQuest* Quest);

	UFUNCTION()
	void LoadQuest(AQuest* Quest, FQuestData QuestData);

	UFUNCTION()
	void ShowSaveIcon();

	UFUNCTION()
	void HideSaveIcon();

	UFUNCTION()
	void OnAsyncSaveFinished(const FString& SlotName, const int32 UserIndex, bool bSuccess);

	UFUNCTION()
	AActor* FindActorByName(FString SearchedName, TSubclassOf<AActor> ActorClass);

	UPROPERTY()
	URPGSaveGame* SavedGame;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> SaveIconWidgetClass;

	UPROPERTY()
	UUserWidget* SaveIconWidget;

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> KilledNPCs;

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> Choices;

	FTimerHandle SaveIconTimer;
};