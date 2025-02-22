// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HeroHUD.generated.h"

class UHeroOverlay;
/**
 * 
 */
UCLASS()
class BLACKWKVSASTROBOT_API AHeroHUD : public AHUD
{
	GENERATED_BODY()

public:

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Overlay)
	TSubclassOf<UHeroOverlay> HeroOverlayClass;

	UPROPERTY()
	UHeroOverlay* HeroOverlay;

public:
	FORCEINLINE UHeroOverlay* GetHeroOverlay() const { return HeroOverlay; }
};
