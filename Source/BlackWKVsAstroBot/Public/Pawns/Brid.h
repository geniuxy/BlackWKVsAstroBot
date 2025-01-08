// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Brid.generated.h"

UCLASS()
class BLACKWKVSASTROBOT_API ABrid : public APawn
{
	GENERATED_BODY()

public:
	ABrid();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:
};
