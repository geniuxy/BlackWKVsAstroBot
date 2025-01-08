// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Brid.h"

ABrid::ABrid()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABrid::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABrid::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

