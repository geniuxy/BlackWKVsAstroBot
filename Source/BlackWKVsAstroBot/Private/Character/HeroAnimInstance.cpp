// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroAnimInstance.h"

#include "Character/Hero.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	HeroCharacter = Cast<AHero>(TryGetPawnOwner());
	if (HeroCharacter)
		HeroMovementComponent = HeroCharacter->GetCharacterMovement();
}

void UHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (HeroMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(HeroMovementComponent->Velocity);
		IsFalling = HeroMovementComponent->IsFalling();
		HeroState = HeroCharacter->GetHeroState();
	}
}
