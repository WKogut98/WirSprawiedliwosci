#pragma once

#include "CoreMinimal.h"
#include "Characters/SpriteCharacter.h"
#include "GameMechanics/SkillTypes.h"
#include "CombatCharacter.generated.h"

class UEffect;
class USkill;
class UCharacterWidgetComponent;
class UAttributeComponent;
class UEffectIcon;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterHoverDelegate, ACombatCharacter*, HoveredCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterEndHoverDelegate, ACombatCharacter*, HoveredCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterClickedDelegate, ACombatCharacter*, ClickedCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDeathDelegate, ACombatCharacter*, DeadCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetReachedDelegate, ACombatCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLocationReachedDelegate, ACombatCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterEndTurnDelegate, ACombatCharacter*, Character);

UCLASS()
class WIRSPRAWIEDLIWOSCI_API ACombatCharacter : public ASpriteCharacter
{
	GENERATED_BODY()
public:
	ACombatCharacter();
	virtual void Tick(float DeltaTime) override;

	//Combat
	UFUNCTION(BlueprintCallable)
	void GetHit(float Damage);

	UFUNCTION(BlueprintCallable)
	void GetHitEffect(UEffect* Effect);

	UFUNCTION(BlueprintCallable)
	void GetHitVortex(float Damage);

	UFUNCTION(BlueprintCallable)
	void ReceiveHealing(float Healing);

	UFUNCTION(BlueprintCallable)
	void IncreaseRage();

	virtual void Die() override;

	UFUNCTION(BlueprintCallable)
	void HandleOnHover();

	UFUNCTION(BlueprintCallable)
	void HandleOnEndHover();

	UFUNCTION(BlueprintCallable)
	void HandleOnClicked();

	UFUNCTION(BlueprintCallable)
	void AddEffect(UEffect* Effect);

	void RemoveNegativeEffects();
	void RemoveEffectTags();
	void AddStatModifiers();
	void RemoveStatModifiers();
	void Dodge();
	void AttackMissed();
	void ReceiveCrit();
	void GetMad();
	void GetStunned();
	void StopWhirling();
	void ReturnFromStun();
	UEffect* GetEffectOfType(EEffectType Type);

	UFUNCTION(BlueprintCallable)
	void UseMana(float Value);

	//UI
	void ShowWidget();
	void HideWidget();
	void ShowArrow();
	void SetArrowToWhite();
	virtual void SetArrowToColor();
	void HideArrow();
	void UpdateEffectIcons();
	void UpdateHealthBar();
	void UpdateManaBar();
	void UpdateRageBar();
	void ShowSpecialInfoText(FString SpecialText);

	virtual void StartTurn();
	void EndTurn();

	//virtual void RemoveSkillsFromRoot();
	//virtual void BeginDestroy() override;

	//Attributes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAttributeComponent* Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* ParticleSpawner;

	UPROPERTY(EditAnywhere)
	TArray<FString> SkillNames;

	template<typename T>
	T Avg(T first, T second);

	UPROPERTY()
	TArray<USkill*> EquippedSkills;

	UPROPERTY()
	USkill* Ult;

	UPROPERTY(BlueprintReadWrite)
	int32 Rank;

	UPROPERTY();
	int32 Turn = 0;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCharacterEndHoverDelegate OnCharacterEndHover;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCharacterHoverDelegate OnCharacterHover;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCharacterClickedDelegate OnCharacterClicked;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnTargetReachedDelegate OnTargetReached;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnLocationReachedDelegate OnLocationReached;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCharacterDeathDelegate OnCharacterDeath;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCharacterEndTurnDelegate OnCharacterEndTurn;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	FVector InitialLocation;

	UPROPERTY()
	FTimerHandle TurnTimer;

	UPROPERTY()
	TMap<FString, int16> StatModifiers;

	//UPROPERTY()
	bool bSelectable = false;

	bool bShouldMoveToTarget = false;
	bool bShouldMoveToLocation = false;

	//getters
	//FORCEINLINE ECharacterActionState GetActionState() const { return ActionState; };
	FORCEINLINE TArray<UEffect*> GetEffects() const { return Effects; };
	FORCEINLINE int32 GetExperience() const { return Experience; };
	FORCEINLINE int32 GetExpToNextLevel() const { return ExpToNextLevel; };

	//setters
	//void SetActionState(ECharacterActionState State);

	virtual void SetExperience(int32 NewExperience);
	FORCEINLINE void SetExpToNextLevel(int32 NewExpToNextLevel) { ExpToNextLevel = NewExpToNextLevel; };
protected:
	virtual void BeginPlay() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void ApplyEffectsPreTurn();
	void ApplyEffectsPostTurn();

	void PlayHealSound();
	void PlayDodgeSound();
	void PlayRageSound();
	void PlayAttackMissedSound();

	void SpawnHealParticles();
	void SpawnEffectParticles(UEffect* Effect);

	UPROPERTY(EditAnywhere)
	TArray<uint8> AvailableRanks;

	UPROPERTY(BlueprintReadOnly)
	TArray<UEffect*> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Stats)
	int32 Experience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int32 ExpToNextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCharacterWidgetComponent* CharacterWidgetComponent;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* WhirlSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* HealSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* DodgeSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* AttackMissedSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* RageSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* MaxRageSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* HealVFX;

	UPROPERTY()
	FTimerHandle SpecialTextTimer;

	UPROPERTY()
	FTimerHandle WhirlTimer;

	UPROPERTY()
	FTimerHandle DeathTimer;
private:
	UPROPERTY()
	bool bWhirling = false;
};