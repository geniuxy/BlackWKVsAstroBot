// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapons/Weapon.h"

#include "Character/Hero.h"


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
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, AttachmentRules, SocketName);
}
