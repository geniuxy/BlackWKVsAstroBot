// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kratos.generated.h"

UCLASS()
class BLACKWKVSASTROBOT_API AKratos : public ACharacter
{
	GENERATED_BODY()

public:
	AKratos();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:
};
