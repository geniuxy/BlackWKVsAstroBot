// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include "BlackWKVsAstroBot/BlackWKVsAstroBot.h"

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

	FVector Location = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();

	DRAW_SPHERE(GetActorLocation());
	DRAW_LINE(Location, Location + ForwardVector * 100);
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
