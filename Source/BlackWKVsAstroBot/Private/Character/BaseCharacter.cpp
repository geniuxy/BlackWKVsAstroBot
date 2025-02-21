// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "BlackWKVsAstroBot/DebugMacros.h"
#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Item/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (Attributes && Attributes->IsAlive() && Hitter)
		DirectionalHitReact(Hitter->GetActorLocation());
	else
		Die();

	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);

	if (HitParticles)
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, ImpactPoint);
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::PlayAttackMontage()
{
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
}

void ABaseCharacter::PlayDeathMontage()
{
}

FVector ABaseCharacter::GetMotionWarpTranslation()
{
	if (CombatTarget)
	{
		const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
		const FVector Location = GetActorLocation();
		const FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
		// DRAW_SPHERE(TargetToMe * WarpTargetDistance + CombatTargetLocation);
		return TargetToMe * WarpTargetDistance + CombatTargetLocation;
	}
	return FVector();
}

FVector ABaseCharacter::GetMotionWarpRotation()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

void ABaseCharacter::AttackEnd()
{
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
		Attributes->ReceiveDamage(DamageAmount);
}

void ABaseCharacter::DirectionalHitReact(const FVector& HitterPos)
{
	const FVector ImpactLower = FVector(HitterPos.X, HitterPos.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLower - GetActorLocation()).GetSafeNormal();
	const FVector Forward = GetActorForwardVector();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0.f)
		Theta *= -1;

	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f)
		Section = FName("FromFront");
	else if (Theta >= -135.f && Theta < -45.f)
		Section = FName("FromLeft");
	else if (Theta >= 45.f && Theta < 135.f)
		Section = FName("FromRight");
	PlayHitReactLargeMontage(Section);

	/*
	DRAW_ARROW(GetActorLocation(), GetActorLocation() + CrossProduct * 60.f, FColor::Purple);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta,
										 FString::Printf(TEXT("Theta: %f, Dir: %s"), Theta, *Section.ToString()));
	DRAW_ARROW(GetActorLocation(), GetActorLocation() + Forward * 60.f, FColor::Red);
	DRAW_ARROW(GetActorLocation(), GetActorLocation() + ToHit * 60.f, FColor::Green);
	*/
}

void ABaseCharacter::SetWeaponCollision(ECollisionEnabled::Type CollisionType)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponCollision())
	{
		EquippedWeapon->GetWeaponCollision()->SetCollisionEnabled(CollisionType);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ABaseCharacter::PlayHitReactLargeMontage(FName HitFromSection)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactLargeMontage)
	{
		AnimInstance->Montage_Play(HitReactLargeMontage);
		AnimInstance->Montage_JumpToSection(HitFromSection, HitReactLargeMontage);
	}
}
