// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Character/HeroTypes.h"
#include "Perception/PawnSensingComponent.h"
#include "Enemy.generated.h"

enum class EEnemyState : uint8;
class AAIController;
class UHealthBarComponent;
class UWidgetComponent;
class UAttributeComponent;

UCLASS()
class BLACKWKVSASTROBOT_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;

	/**
	 *  Patrol
	 */
	UPROPERTY()
	AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category="AI Navigation")
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category="AI Navigation")
	float PatrolWalkSpeed = 30.f;

	UPROPERTY(EditAnywhere, Category="AI Navigation")
	float ChasingSpeed = 300.f;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	void CheckPatrolTarget();

	/**
	 * Combat
	 */

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category=Combat)
	float AttackWaitMin = 0.5f;
	UPROPERTY(EditAnywhere, Category=Combat)
	float AttackWaitMax = 1.f;

	void CheckCombatTarget();

	/**
	 * AI Behaviour
	 */
	void LoseInterest();
	void StartPatrol();
	void StartChase();
	void StartAttack();
	bool CanChase();

protected:
	UFUNCTION()
	virtual void BeginPlay() override;

	void InitializeEnemy();

	virtual void Attack() override;

	virtual void PlayAttackMontage() override;
	virtual void PlayDeathMontage() override;

	virtual void Die() override;

	virtual void HandleDamage(float DamageAmount) override;

	virtual void AttackEnd() override;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	bool InTargetRange(AActor* Target, double Radius);

	void MoveToTarget(AActor* Target);

	AActor* ChoosingNextPatrolTarget();

	UFUNCTION()
	void PawnSeen(APawn* Pawn);

	/**
	 * Combat
	 */
	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;

	virtual bool CanAttack() override;
	bool CanPatrol();

private:
	UPROPERTY(EditAnywhere)
	UHealthBarComponent* HealthBarComponent;

	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

public:
};
