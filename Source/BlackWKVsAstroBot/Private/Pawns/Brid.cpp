// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Brid.h"

#include "Components/CapsuleComponent.h"

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
}

void ABrid::MoveForward(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("value is: %f"), Value);
}

void ABrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABrid::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"),this, &ABrid::MoveForward);
}
