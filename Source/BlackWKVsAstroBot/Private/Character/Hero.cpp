// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/Weapons/Weapon.h"
#include "Components/BoxComponent.h"

AHero::AHero()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("HeroCameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HeroViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	ViewCamera->bUsePawnControlRotation = false;
}

void AHero::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

	Tags.Add(FName("Hero"));
}

void AHero::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_UnOccupied) return;

	const FVector2D MoveAxisValue = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// 这一步是 在乘以旋转矩阵后 的值
		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardVector, MoveAxisValue.Y);
		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightVector, MoveAxisValue.X);
	}
}

void AHero::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void AHero::Equip()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		HeroState = EHeroState::EHS_EquippedOneHandedWeapon;
		EquippedWeapon = OverlappingWeapon;
		OverlappingItem = nullptr;
	}
	else
	{
		if (CanDisArm())
		{
			PlayEquipMontage(FName("UnEquip"));
			HeroState = EHeroState::EHS_UnEquipped;
			ActionState = EActionState::EAS_Equipping;
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			HeroState = EHeroState::EHS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_Equipping;
		}
	}
}

void AHero::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void AHero::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const float CurrentTime = GetWorld()->GetTimeSeconds();

		// check combo attack
		if (CurrentTime - LastAttackTime < AttackWindow)
			AttackCount = (AttackCount + 1) % 3;
		else
			AttackCount = 0;
		LastAttackTime = CurrentTime;

		// 打印last attack time
		UE_LOG(LogTemp, Warning, TEXT("LastAttackTime: %f"), LastAttackTime);
		// 打印当前攻击次数
		UE_LOG(LogTemp, Warning, TEXT("AttackCount: %d"), AttackCount);

		FName SectionName;
		switch (AttackCount)
		{
		case 0:
			SectionName = FName("Attack_Downward");
			break;
		case 1:
			SectionName = FName("Attack_Horizontal");
			break;
		case 2:
			SectionName = FName("Attack_Backhand");
			break;
		default:
			SectionName = FName("Attack_Downward");
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AHero::BackToUnoccupiedState()
{
	ActionState = EActionState::EAS_UnOccupied;
}

void AHero::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AHero::DisArm()
{
	if (EquippedWeapon)
		EquippedWeapon->AttachWeaponTo(GetMesh(), FName("SpineSocket"));
}

void AHero::Arm()
{
	if (EquippedWeapon)
		EquippedWeapon->AttachWeaponTo(GetMesh(), FName("RightHandSocket"));
}

void AHero::SetWeaponCollision(ECollisionEnabled::Type CollisionType)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponCollision())
	{
		EquippedWeapon->GetWeaponCollision()->SetCollisionEnabled(CollisionType);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

bool AHero::CanAttack()
{
	return ActionState == EActionState::EAS_UnOccupied && HeroState != EHeroState::EHS_UnEquipped;
}

bool AHero::CanDisArm()
{
	return ActionState == EActionState::EAS_UnOccupied && HeroState != EHeroState::EHS_UnEquipped;
}

bool AHero::CanArm()
{
	return ActionState == EActionState::EAS_UnOccupied && HeroState == EHeroState::EHS_UnEquipped && EquippedWeapon;
}

void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHero::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHero::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AHero::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AHero::Equip);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AHero::Attack);
	}
}
