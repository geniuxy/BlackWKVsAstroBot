// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Kratos.h"

AKratos::AKratos()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AKratos::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKratos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKratos::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

