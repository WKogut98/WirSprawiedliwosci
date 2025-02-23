// Po kim ty taki chitry na prawa autorskie?!


#include "UI/DamageIndicatorWidgetComponent.h"
#include "UI/DamageIndicator.h"

void UDamageIndicatorWidgetComponent::InitializeDamageIndicator()
{
	if (DamageIndicator == nullptr)
	{
		DamageIndicator = Cast<UDamageIndicator>(GetUserWidgetObject());
	}
}

void UDamageIndicatorWidgetComponent::DisplayDamage(float Damage)
{
	InitializeDamageIndicator();
	if (DamageIndicator)
	{
		DamageIndicator->DisplayDamage(Damage);
	}
}

void UDamageIndicatorWidgetComponent::DisplayHealing(float Healing)
{
	InitializeDamageIndicator();
	if (DamageIndicator)
	{
		DamageIndicator->DisplayHealing(Healing);
	}
}
