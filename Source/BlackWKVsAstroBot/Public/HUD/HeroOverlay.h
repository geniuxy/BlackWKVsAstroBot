// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeroOverlay.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class BLACKWKVSASTROBOT_API UHeroOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthBarPercent(float Percent);
	void SetStaminaBarPercent(float Percent);
	void SetCoinText(int32 CoinNum);
	void SetSoulText(int32 SoulNum);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulText;
};
