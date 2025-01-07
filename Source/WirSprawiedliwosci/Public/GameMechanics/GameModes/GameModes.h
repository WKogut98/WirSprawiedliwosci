#pragma once

UENUM(BlueprintType)
enum class ECurrentMode :uint8
{
	ECM_Exploration UMETA(DisplayName = "Exploration"),
	ECM_ExplorationInterior UMETA(DisplayName = "Exploration Interior"),
	ECM_Combat UMETA(DisplayName = "Combat")
};
