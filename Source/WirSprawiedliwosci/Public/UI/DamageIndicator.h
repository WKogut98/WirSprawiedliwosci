// Po kim ty taki chitry na prawa autorskie?!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageIndicator.generated.h"

/**
 * 
 */

class UTextBlock;
class UWidgetAnimation;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UDamageIndicator : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void DisplayDamage(float Damage);
	void DisplayHealing(float Healing);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* PopUp;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* PopUpGreen;
};
