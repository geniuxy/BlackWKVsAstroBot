// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class AAIController;
class UHealthBarComponent;
class UWidgetComponent;
class UAttributeComponent;

UCLASS()
class BLACKWKVSASTROBOT_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	void DirectionalHitReact(const FVector& ImpactPoint);


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;

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

protected:
	virtual void BeginPlay() override;

	void Die();
	
	bool InTargetRange(AActor* Target, double Radius);

	/**
	 * Montage functions
	 */
	void PlayHitReactLargeMontage(FName HitFromSection);

	/**
	 * Combat
	 */
	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UAttributeComponent* Attributes;

	UPROPERTY(EditAnywhere)
	UHealthBarComponent* HealthBarComponent;

	/** 
	 * Hit React Large
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactLargeMontage;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

	/** 
	 * Death
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

public:
};
