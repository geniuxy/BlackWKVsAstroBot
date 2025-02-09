// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"

#include "BlackWKVsAstroBot/DebugMacros.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(GetRootComponent());

	// When chasing a character, can face the direction of the movement.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarComponent)
		HealthBarComponent->SetVisibility(false);

	EnemyController = Cast<AAIController>(GetController());
	if (EnemyController && PatrolTarget)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PatrolTarget);
		MoveRequest.SetAcceptanceRadius(15.f);
		FNavPathSharedPtr NavPath;
		EnemyController->MoveTo(MoveRequest, &NavPath);
		TArray<FNavPathPoint> PathPoints = NavPath->GetPathPoints();
		for (auto& Point : PathPoints)
		{
			const FVector& Location = Point.Location;
			DRAW_SPHERE_COLOR(Location, FColor::Red);
		}
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget)
	{
		if (!InTargetRange(CombatTarget, CombatRadius))
		{
			CombatTarget = nullptr;
			if (HealthBarComponent)
				HealthBarComponent->SetVisibility(false);
		}
	}
	if (PatrolTarget && EnemyController)
	{
		if (InTargetRange(PatrolTarget, PatrolRadius))
		{
			TArray<AActor*> ValidTargets;
			for (AActor* Target : PatrolTargets)
			{
				if (Target != PatrolTarget)
					ValidTargets.AddUnique(Target);
			}

			const int32 NumValidTargets = ValidTargets.Num();
			if (NumValidTargets > 0)
			{
				const int32 TargetSection = FMath::RandRange(0, NumValidTargets - 1);
				AActor* NextTarget = ValidTargets[TargetSection];
				PatrolTarget = NextTarget;

				FAIMoveRequest MoveRequest;
				MoveRequest.SetGoalActor(PatrolTarget);
				MoveRequest.SetAcceptanceRadius(15.f);
				EnemyController->MoveTo(MoveRequest);
			}
		}
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	// DRAW_SPHERE_COLOR(ImpactPoint, FColor::Emerald);
	if (HealthBarComponent)
		HealthBarComponent->SetVisibility(true);
	if (Attributes && Attributes->IsAlive())
		DirectionalHitReact(ImpactPoint);
	else
		Die();

	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);

	if (HitParticles)
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, ImpactPoint);
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector ImpactLower = FVector(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
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

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		const int32 NumSections = 3; // 动画段的数量，可以动态调整
		const int32 RandomSectionIndex = FMath::RandRange(1, NumSections);
		const FString RandomSectionName = FString::Printf(TEXT("Death%d"), RandomSectionIndex);
		AnimInstance->Montage_JumpToSection(*RandomSectionName, DeathMontage);
	}
	if (HealthBarComponent)
		HealthBarComponent->SetVisibility(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	DRAW_SPHERE_SINGLE_FRAME(GetActorLocation());
	DRAW_SPHERE_SINGLE_FRAME(Target->GetActorLocation());
	return DistanceToTarget <= Radius;
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
                         AActor* DamageCauser)
{
	if (Attributes && HealthBarComponent)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarComponent->SetHealthPercent(Attributes->GetHealthPercent());
	}
	CombatTarget = EventInstigator->GetPawn();
	return DamageAmount;
}

void AEnemy::PlayHitReactLargeMontage(FName HitFromSection)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactLargeMontage)
	{
		AnimInstance->Montage_Play(HitReactLargeMontage);
		AnimInstance->Montage_JumpToSection(HitFromSection, HitReactLargeMontage);
	}
}
