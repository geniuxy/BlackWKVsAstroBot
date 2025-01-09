// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Brid.h"

#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ABrid::ABrid()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	Capsule->SetCapsuleHalfHeight(30.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	BridMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BridMesh"));
	BridMesh->SetupAttachment(GetRootComponent());

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABrid::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BridMappingContext, 0);
		}
	}
}

void ABrid::MoveForward(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("value is: %f"), Value);
}

void ABrid::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();

	if (DirectionValue != 0.f)
	{
		FVector ForwardVector = GetActorForwardVector();
		AddMovementInput(ForwardVector, DirectionValue);
	}
}

void ABrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABrid::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABrid::Move);
	}
}
