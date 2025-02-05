// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

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

protected:
	virtual void BeginPlay() override;

	/**
	 * Montage functions
	 */
	void PlayHitReactLargeMontage(FName HitFromSection);

private:
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;
	
	/** 
	 * Hit React Large
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactLargeMontage;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

public:
};
