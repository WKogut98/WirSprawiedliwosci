#pragma once
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	EDT_Normal UMETA(DisplayName = "Normal"),
	EDT_Slap UMETA(DisplayName = "Slap"),
	EDT_Bonk UMETA(DisplayName = "Bonk"),
	EDT_DarkMagic UMETA(DisplayName = "Dark Magic"),
	EDT_BrightMagic UMETA(DisplayName = "Bright Magic"),
	EDT_AOE UMETA(DisplayName = "Area Of Effect"),
	EDT_Splash UMETA(DisplayName = "Splash"),
	EDT_Chemical UMETA(DisplayName = "Chemical"),
};