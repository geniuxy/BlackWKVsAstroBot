// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeroTypes.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimInstance.generated.h"

class AHero;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class BLACKWKVSASTROBOT_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	AHero* HeroCharacter;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	UCharacterMovementComponent* HeroMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category="Movement | Hero State")
	EHeroState HeroState;
};
