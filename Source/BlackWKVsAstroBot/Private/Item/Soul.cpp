// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Soul.h"

#include "Interfaces/PickupInterface.h"

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->AddSouls(this);
		SpawnPickUpSound();
		SpawnPickUpEffect();
		Destroy();
	}
}
