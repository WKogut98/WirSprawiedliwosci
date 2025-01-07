#include "UI/EffectIcon.h"
#include "GameMechanics/Effect.h"
#include "Components/Image.h"

void UEffectIcon::SetNewEffect(UEffect* NewEffect)
{
	Effect = NewEffect;
	if (Effect && Effect->EffectIcon)
	{
		FString TooltipString = Effect->EffectName + TEXT(": ");
		if (Effect->GetValue() != 0)
		{
			TooltipString.Append(FString::SanitizeFloat(FMath::Abs(Effect->GetValue()), 0) + TEXT(" przez "));
		}
		if (Effect->GetDuration() != 0)
		{
			FString Turns =  Effect->GetDuration() == 1 ? TEXT(" tur\u0119.") : TEXT(" tur(y).");
			TooltipString.Append(FString::FromInt(Effect->GetDuration()) + Turns);
		}
		Icon->SetBrushFromTexture(Effect->EffectIcon, false);
		SetToolTipText(FText::FromString(TooltipString));
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UEffectIcon::ClearIcon()
{
	Effect = nullptr;
	Icon->SetBrushFromTexture(nullptr, false);
	SetVisibility(ESlateVisibility::Hidden);
}
