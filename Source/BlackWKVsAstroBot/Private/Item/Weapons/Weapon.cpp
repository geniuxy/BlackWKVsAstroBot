// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapons/Weapon.h"

#include "Character/Hero.h"

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AHero* Hero = Cast<AHero>(OtherActor);
	if (Hero)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		ItemMesh->AttachToComponent(Hero->GetMesh(), AttachmentRules, FName("RightHandSocket"));
	}
}

void AWeapon::ExitSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::ExitSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
