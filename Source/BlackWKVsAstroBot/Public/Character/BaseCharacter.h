// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class UAttributeComponent;
class AWeapon;

UCLASS()
class BLACKWKVSASTROBOT_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	virtual void BeginPlay() override;

	virtual void Attack();

	UFUNCTION(BlueprintCallable)
	FVector GetMotionWarpTranslation();

	UFUNCTION(BlueprintCallable)
	FVector GetMotionWarpRotation();

	UPROPERTY(EditAnywhere, Category = Combat)
	double WarpTargetDistance = 75.f;
	
	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* CombatTarget;

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();

	virtual bool CanAttack();

	virtual void Die();

	virtual void HandleDamage(float DamageAmount);

	/**
	 * Hit
	 */
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

	void DirectionalHitReact(const FVector& HitterPos);

	/**
	 * Weapon
	 */
	UPROPERTY(VisibleInstanceOnly, Category = Weapon)
	AWeapon* EquippedWeapon;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type CollisionType);

	/**
	 * Components
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UAttributeComponent* Attributes;

	/**
	 * Montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactLargeMontage;

	/**
	 * Montage functions
	 */
	virtual void PlayHitReactLargeMontage(FName HitFromSection);
	virtual void PlayAttackMontage();
	void StopAttackMontage();
	virtual void PlayDeathMontage();

private:

public:
};
