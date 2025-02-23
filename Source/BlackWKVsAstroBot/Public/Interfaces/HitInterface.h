// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLACKWKVSASTROBOT_API IHitInterface
{
	GENERATED_BODY()

public:
	// if pure virtual function of interface, need + " = 0"
	// virtual void GetHit(const FVector& ImpactPoint) = 0;

	// 为了在蓝图类中播放声音，将此改为BlueprintNativeEvent
	// BlueprintNativeEvent是可以在蓝图类中重写C++方法
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint, AActor* Hitter);
};
