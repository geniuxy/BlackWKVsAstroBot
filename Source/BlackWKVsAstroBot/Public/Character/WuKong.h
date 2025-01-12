// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WuKong.generated.h"

UCLASS()
class BLACKWKVSASTROBOT_API AWuKong : public ACharacter
{
	GENERATED_BODY()

public:
	AWuKong();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:
};
