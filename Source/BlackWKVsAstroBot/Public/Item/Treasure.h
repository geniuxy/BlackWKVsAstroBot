// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class BLACKWKVSASTROBOT_API ATreasure : public AItem
{
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold;

public:
	FORCEINLINE int32 GetValueOfGold() const {return Gold;}
};
