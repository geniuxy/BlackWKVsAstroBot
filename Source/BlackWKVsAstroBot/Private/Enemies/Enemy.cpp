// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"

#include "BlackWKVsAstroBot/DebugMacros.h"
#include "Character/Hero.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Item/Weapons/Weapon.h"
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

	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(GetRootComponent());

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensingComponent->SightRadius = 4000.f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);

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
	MoveToTarget(PatrolTarget);

	if (PawnSensingComponent)
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	InitializeEnemy();

	Tags.Add(FName("Enemy"));
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EnemyState == EEnemyState::EES_Dead) return;
	if (EnemyState > EEnemyState::EES_Patrolling)
		CheckCombatTarget();
	else
		CheckPatrolTarget();
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::InitializeEnemy()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* SpawnWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		SpawnWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = SpawnWeapon;
	}
}

void AEnemy::Attack()
{
	Super::Attack();
	
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
		CombatTarget = nullptr;
	if (CombatTarget == nullptr) return;
	
	EnemyState = EEnemyState::EES_Engaged;

	PlayAttackMontage();
}

void AEnemy::PlayAttackMontage()
{
	Super::PlayAttackMontage();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);

		const int32 RandomAttackIndex = FMath::RandRange(1, 3);
		FName SectionName = FName(*FString::Printf(TEXT("Attack%d"), RandomAttackIndex));
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	// DRAW_SPHERE_COLOR(ImpactPoint, FColor::Emerald);
	if (EnemyState != EEnemyState::EES_Dead && HealthBarComponent)
		HealthBarComponent->SetVisibility(true);
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	GetWorldTimerManager().ClearTimer(AttackTimer);
	SetWeaponCollision(ECollisionEnabled::NoCollision);

	StopAttackMontage();
}

void AEnemy::Die()
{
	Super::Die();
	
	EnemyState = EEnemyState::EES_Dead;

	GetWorldTimerManager().ClearTimer(AttackTimer);

	if (HealthBarComponent)
		HealthBarComponent->SetVisibility(false);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponCollision(ECollisionEnabled::NoCollision);
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if (HealthBarComponent)
		HealthBarComponent->SetHealthPercent(Attributes->GetHealthPercent());
}

void AEnemy::AttackEnd()
{
	Super::AttackEnd();

	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::Destroyed()
{
	Super::Destroyed();

	if (EquippedWeapon)
		EquippedWeapon->Destroy();
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	if (HealthBarComponent)
		HealthBarComponent->SetVisibility(false);
}

void AEnemy::StartPatrol()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrolWalkSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemy::StartChase()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

void AEnemy::StartAttack()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackWaitTime = FMath::RandRange(AttackWaitMin, AttackWaitMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackWaitTime);
}

bool AEnemy::CanPatrol()
{
	return !InTargetRange(CombatTarget, CombatRadius) && EnemyState != EEnemyState::EES_Dead;
}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		InTargetRange(CombatTarget, AttackRadius) &&
		EnemyState != EEnemyState::EES_Attacking &&
		EnemyState != EEnemyState::EES_Engaged &&
		EnemyState != EEnemyState::EES_Dead;
	return bCanAttack;
}

bool AEnemy::CanChase()
{
	bool bCanChase =
		!InTargetRange(CombatTarget, AttackRadius) &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState != EEnemyState::EES_Dead;
	return bCanChase;
}

void AEnemy::CheckCombatTarget()
{
	if (CanPatrol())
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		LoseInterest();
		if (EnemyState != EEnemyState::EES_Engaged)
			StartPatrol();
		// UE_LOG(LogTemp, Warning, TEXT("back to patrol target, lose interest!"));
	}
	else if (CanChase())
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		if (EnemyState != EEnemyState::EES_Engaged)
			StartChase();
		// UE_LOG(LogTemp, Warning, TEXT("chasing target!"));
	}
	else if (CanAttack())
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		StartAttack();
		// UE_LOG(LogTemp, Warning, TEXT("Attack!"));
	}
	else if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
		StartPatrol();
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosingNextPatrolTarget();
		const float RandomPatrolWaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, RandomPatrolWaitTime);
	}
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

AActor* AEnemy::ChoosingNextPatrolTarget()
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
		return ValidTargets[TargetSection];
	}
	return nullptr;
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if (EnemyState != EEnemyState::EES_Patrolling) return;
	if (SeenPawn->ActorHasTag(FName("Hero")) && !SeenPawn->ActorHasTag(FName("Dead")))
	{
		CombatTarget = SeenPawn;
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		StartChase();
		UE_LOG(LogTemp, Warning, TEXT("see the character, chasing!"));
	}
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	DRAW_SPHERE_SINGLE_FRAME(GetActorLocation());
	DRAW_SPHERE_SINGLE_FRAME(Target->GetActorLocation());
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	EnemyController->MoveTo(MoveRequest);
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
                         AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();

	if (InTargetRange(CombatTarget, AttackRadius))
		EnemyState = EEnemyState::EES_Attacking;
	else
		StartChase();
	
	return DamageAmount;
}
