#pragma once

UENUM(BlueprintType)
enum class EHeroState : uint8
{
	EHS_UnEquipped UMETA(DisplayName = "UnEquipped"),
	EHS_EquippedOneHandedWeapon UMETA(DisplayName = "Equip One-Handed Weapon"),
	EHS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equip Two-Handed Weapon"),
};
