// Po kim ty taki chitry na prawa autorskie?!

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageIndicatorWidgetComponent.generated.h"

/**
 * 
 */
class UDamageIndicator;
UCLASS()
class WIRSPRAWIEDLIWOSCI_API UDamageIndicatorWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void InitializeDamageIndicator();
	void DisplayDamage(float Damage);
	void DisplayHealing(float Healing);
	UDamageIndicator* DamageIndicator;
};
