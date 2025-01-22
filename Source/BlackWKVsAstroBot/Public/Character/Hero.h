// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeroTypes.h"
#include "GameFramework/Character.h"
#include "Hero.generated.h"

class AItem;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class BLACKWKVSASTROBOT_API AHero : public ACharacter
{
	GENERATED_BODY()

public:
	AHero();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputMappingContext* KratosMappingContext;

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
	void Attack();

	/**
	 * Montage functions
	 */

	void PlayAttackMontage();

	UFUNCTION(BlueprintCallable)
	void FinishAttackState();

private:
	EHeroState HeroState = EHeroState::EHS_UnEquipped;
	EActionState ActionState = EActionState::EAS_UnEquipped;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	/** 
	 * Attack
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	int32 AttackCount;

	float LastAttackTime;

	const float AttackWindow = 2.0f;

	bool CanAttack();

public:
	FORCEINLINE void SetOverlappingWeapon(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE EHeroState GetHeroState() const { return HeroState; }
};
