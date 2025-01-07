#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_InDialogue UMETA(DisplayName = "In Dialogue"),
	ECS_Busy UMETA(DisplayName = "Busy")
};

//UENUM(BlueprintType)
//enum class ECharacterActionState : uint8
//{
//	ECAS_NotInCombat UMETA(DisplayName = "Not In Combat"),
//	ECAS_InCombat UMETA(DisplayName = "In Combat"),
//	ECAS_Stunned UMETA(DisplayName = "Stunned")
//};