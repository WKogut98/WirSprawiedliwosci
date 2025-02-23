// Po kim ty taki chitry na prawa autorskie?!


#include "UI/DamageIndicator.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UDamageIndicator::DisplayDamage(float Damage)
{
	if (DamageText && PopUp)
	{
		int32 DamageInt = FMath::RoundToInt(Damage);
		DamageText->SetText(FText::FromString(FString::FromInt(DamageInt)));
		PlayAnimation(PopUp);
	}
}

void UDamageIndicator::DisplayHealing(float Healing)
{
	if (DamageText && PopUp)
	{
		int32 HealingInt = FMath::RoundToInt(Healing);
		DamageText->SetText(FText::FromString(FString::FromInt(HealingInt)));
		PlayAnimation(PopUpGreen);
	}
}
