// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "HeroTypes.h"
#include "Hero.generated.h"

class AWeapon;
class AItem; 
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class BLACKWKVSASTROBOT_API AHero : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AHero();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputMappingContext* MappingContext;

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

	virtual void BackToUnoccupiedState() override;

	void PlayEquipMontage(FName SectionName);

	UFUNCTION(BlueprintCallable)
	void DisArm();

	UFUNCTION(BlueprintCallable)
	void Arm();

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
