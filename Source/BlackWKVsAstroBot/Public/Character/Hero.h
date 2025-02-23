// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "HeroTypes.h"
#include "Interfaces/PickupInterface.h"
#include "Item/Treasure.h"
#include "Hero.generated.h"

class UHeroOverlay;
class AWeapon;
class AItem;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class BLACKWKVSASTROBOT_API AHero : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	AHero();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual void SetOverlappingItem(AItem* Item) override;

	virtual void AddGolds(ATreasure* Treasure) override;
	
	virtual void AddSouls(ASoul* Soul) override;

protected:
	virtual void BeginPlay() override;

	void InitializeEnhancedInput();
	void InitializeHeroOverlay();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputMappingContext* MappingContext;

	UPROPERTY()
	UHeroOverlay* HeroOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* AttackAction;

	/**
	 * callbacks for inputs
	 */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip();
	virtual void Attack() override;

	/**
	 * Montage functions
	 */

	virtual void PlayAttackMontage() override;

	void PlayEquipMontage(FName SectionName);

	virtual void AttackEnd() override;

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void DisArm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	void UpdateHealthBar();
	
	virtual void Die() override;

private:
	EHeroState HeroState = EHeroState::EHS_UnEquipped;
	EActionState ActionState = EActionState::EAS_UnOccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	/** 
	 * Attack
	 */
	int32 AttackCount;

	float LastAttackTime;

	const float AttackWindow = 2.0f;

	virtual bool CanAttack() override;

	/**
	 * Equip
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	bool CanDisArm();

	bool CanArm();

public:
	FORCEINLINE void SetOverlappingWeapon(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE EHeroState GetHeroState() const { return HeroState; }
};
