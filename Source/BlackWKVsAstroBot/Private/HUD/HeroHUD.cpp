// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HeroHUD.h"

#include "Blueprint/UserWidget.h"
#include "HUD/HeroOverlay.h"

void AHeroHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && HeroOverlayClass)
		{
			HeroOverlay = CreateWidget<UHeroOverlay>(Controller, HeroOverlayClass);
			HeroOverlay->AddToViewport();
		}
	}
}
