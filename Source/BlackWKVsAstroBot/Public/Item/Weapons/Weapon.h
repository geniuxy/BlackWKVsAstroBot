// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class BLACKWKVSASTROBOT_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	virtual void Equip(USceneComponent* InParent, FName SocketName);

	void AttachWeaponTo(USceneComponent* InParent, FName SocketName);

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult) override;

	virtual void ExitSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
