#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributesMenu.generated.h"

class UTextBlock;
class ULevelUpButton;
class AAlly;

UCLASS()
class WIRSPRAWIEDLIWOSCI_API UAttributesMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetNewAlly(AAlly* NewAlly);

	UFUNCTION()
	void UpdateElements();

	void UpdateAttributes();
	void UpdateLevelUpButtons();
	TArray<ULevelUpButton*> GetLevelUpButtonsAsArray();

	UPROPERTY()
	AAlly* CurrentAlly;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AttributePoints;

	//Stats
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Vitality;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Magic;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Genocide;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Healing;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Defence;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Speed;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Dodge;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Luck;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Accuracy;


	//Resistances
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StunResistance;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OppressionResistance;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StinkResistance;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DisgraceResistance;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DrunkResistance;

	//Level Up Buttons
	UPROPERTY(meta = (BindWidget))
	ULevelUpButton* LevelUpBtnVitality;

	UPROPERTY(meta = (BindWidget))
	ULevelUpButton* LevelUpBtnMagic;

	UPROPERTY(meta = (BindWidget))
	ULevelUpButton* LevelUpBtnGenocide;

	UPROPERTY(meta = (BindWidget))
	ULevelUpButton* LevelUpBtnHealing;

	UPROPERTY(meta = (BindWidget))
	ULevelUpButton* LevelUpBtnDefence;

	UPROPERTY(meta = (BindWidget))
	ULevelUpButton* LevelUpBtnDodge;

	UPROPERTY(meta = (BindWidget))
	ULevelUpButton* LevelUpBtnSpeed;

	UPROPERTY(meta = (BindWidget))
	ULevelUpButton* LevelUpBtnLuck;

	UPROPERTY(meta = (BindWidget))
	ULevelUpButton* LevelUpBtnAccuracy;
};
