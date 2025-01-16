// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KratosAnimInstance.h"

#include "Character/Kratos.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UKratosAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	KratosCharacter = Cast<AKratos>(TryGetPawnOwner());
	if (KratosCharacter)
		KratosMovementComponent = KratosCharacter->GetCharacterMovement();
}

void UKratosAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (KratosMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(KratosMovementComponent->Velocity);
		IsFalling = KratosMovementComponent->IsFalling();
	}
}
