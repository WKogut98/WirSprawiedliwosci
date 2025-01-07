#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelUpButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatLeveledUpDelegate);

class AAlly;
class UAttributeComponent;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API ULevelUpButton : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void LevelUpStat();

	UPROPERTY(EditAnywhere)
	FString Attribute;

	UPROPERTY()
	AAlly* Ally;

	FOnStatLeveledUpDelegate OnStatLeveledUp;
};
