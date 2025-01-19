// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroAnimInstance.h"

#include "Character/Hero.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	KratosCharacter = Cast<AHero>(TryGetPawnOwner());
	if (KratosCharacter)
		KratosMovementComponent = KratosCharacter->GetCharacterMovement();
}

void UHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (KratosMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(KratosMovementComponent->Velocity);
		IsFalling = KratosMovementComponent->IsFalling();
	}
}
