#pragma once

UENUM(BlueprintType)
enum class EHeroState : uint8
{
	EHS_UnEquipped UMETA(DisplayName = "UnEquipped"),
	EHS_EquippedOneHandedWeapon UMETA(DisplayName = "Equip One-Handed Weapon"),
	EHS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equip Two-Handed Weapon"),
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_UnOccupied UMETA(DisplayName = "UnOccupied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Equipping UMETA(DisplayName = "Equipping"),
	EAS_Dodging UMETA(DisplayName = "Dodging"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "No State"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")
};
