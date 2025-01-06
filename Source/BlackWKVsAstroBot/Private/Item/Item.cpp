// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include "BlackWKVsAstroBot/DebugMacros.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Begin Play!"));

	SetActorLocation(FVector(9360.f, 600.f, 3700.f));
	SetActorRotation(FRotator(45, 0 , 0));

	FVector Location = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();

	DRAW_SPHERE(Location);
	// DRAW_LINE(Location, Location + ForwardVector * 100);
	// DRAW_POINT(Location + ForwardVector * 100);
	DRAW_VECTOR(Location, Location + ForwardVector * 100);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine)
	{
		FString Name = GetName();
		FString Message = FString::Printf(TEXT("Item Name: %s"), *Name);
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, Message);
		UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *Name);
	}
}
