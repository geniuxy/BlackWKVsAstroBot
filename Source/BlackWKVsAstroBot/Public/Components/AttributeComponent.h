// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLACKWKVSASTROBOT_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void AddCurrentSouls(int32 ValueOfSoul);

	void AddCurrentGolds(int32 ValueOfGold);

	void RegenStamina(float DeltaTime);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float Stamina = 120.f;

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float MaxStamina = 120.f;

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	int32 CurrentCoin = 0.f;

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	int32 CurrentSoul = 0.f;

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float DodgeCost = 20.f;
	
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float RegenStaminaRate = 5.f;
public:
	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaCost);
	float GetHealthPercent();
	float GetStaminaPercent();
	bool IsAlive();
	FORCEINLINE int32 GetCurrentCoin() const {return CurrentCoin;}
	FORCEINLINE int32 GetCurrentSoul() const {return CurrentSoul;}
	FORCEINLINE float GetStamina() const {return Stamina;}
	FORCEINLINE float GetDodgeCost() const {return DodgeCost;}
};
