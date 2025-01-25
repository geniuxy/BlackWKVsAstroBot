// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapons/Weapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AWeapon::AWeapon()
{
	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponCollision->SetupAttachment(GetRootComponent());
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::ExitSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::ExitSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::Equip(USceneComponent* InParent, FName SocketName)
{
	AttachWeaponTo(InParent, SocketName);
	ItemState = EItemState::EIS_Equipped;
	if (Sphere)
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::AttachWeaponTo(USceneComponent* InParent, FName SocketName)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, AttachmentRules, SocketName);
}
