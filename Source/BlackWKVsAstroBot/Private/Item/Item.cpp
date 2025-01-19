// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"

#include "Character/Hero.h"
#include "Components/SphereComponent.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	SetRootComponent(ItemMesh);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::ExitSphereOverlap);
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult)
{
	AHero* Hero = Cast<AHero>(OtherActor);
	if (Hero)
		Hero->SetOverlappingWeapon(this);
}

void AItem::ExitSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AHero* Hero = Cast<AHero>(OtherActor);
	if (Hero)
		Hero->SetOverlappingWeapon(nullptr);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	// float MoveSpeed = 50.f;
	// float RotatorSpeed = 45.f;
	// float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);
	// AddActorWorldOffset(FVector(MoveSpeed * DeltaTime, 0.f, DeltaZ));
	// AddActorWorldRotation(FRotator(0.f, RotatorSpeed * DeltaTime, 0.f));
	// DRAW_SPHERE_SINGLE_FRAME(GetActorLocation());
	// DRAW_VECTOR_SINGLE_FRAME(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);
	// DRAW_POINT_SINGLE_FRAME(Avg<FVector>(GetActorLocation(), FVector::ZeroVector));
}
