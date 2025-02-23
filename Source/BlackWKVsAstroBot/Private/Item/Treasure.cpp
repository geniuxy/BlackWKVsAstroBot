// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Treasure.h"

#include "Character/Hero.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult)
{
	AHero* Hero = Cast<AHero>(OtherActor);
	if (Hero)
	{
		SpawnPickUpSound();
		Destroy();
	}
}
