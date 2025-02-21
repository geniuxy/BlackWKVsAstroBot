// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HeroOverlay.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHeroOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthBar)
		HealthBar->SetPercent(Percent);
}

void UHeroOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaBar)
		StaminaBar->SetPercent(Percent);
}

void UHeroOverlay::SetCoinText(int32 CoinNum)
{
	if (CoinText)
	{
		const FString CoinString = FString::Printf(TEXT("%d"), CoinNum);
		const FText TextOfCoinNum = FText::FromString(CoinString);
		CoinText->SetText(TextOfCoinNum);
	}
}

void UHeroOverlay::SetSoulText(int32 SoulNum)
{
	if (SoulText)
	{
		const FString SoulString = FString::Printf(TEXT("%d"), SoulNum);
		const FText TextOfSoulNum = FText::FromString(SoulString);
		SoulText->SetText(TextOfSoulNum);
	}
}
