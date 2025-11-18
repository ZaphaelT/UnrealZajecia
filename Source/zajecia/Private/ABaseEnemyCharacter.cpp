// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/PickableWeapon.h"
#include "ABaseEnemyCharacter.h"
#include "AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"


AABaseEnemyCharacter::AABaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SightRadius = 3000.0f; // Widzi na 15 metrów
	PawnSensingComp->SetPeripheralVisionAngle(90.0f); // K¹t widzenia

	PawnState = EPawnState::EPS_Idle;
}

void AABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AttributeComponent)
	{
		AttributeComponent->OnDeath.AddDynamic(this, &AABaseEnemyCharacter::Die);
	}
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AABaseEnemyCharacter::OnSeePawn);
	}
	if (DefaultWeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		APickableWeapon* SpawnedWeapon = GetWorld()->SpawnActor<APickableWeapon>(DefaultWeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);

		if (SpawnedWeapon)
		{
			EquippedWeapon = SpawnedWeapon;
			EquippedWeapon->GetRootComponent()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
			EquippedWeapon->SetActorEnableCollision(false);
		}
	}
}

void AABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PawnState == EPawnState::EPS_Dead || PawnState == EPawnState::EPS_Hit) return;

	if (CombatTarget && PawnState != EPawnState::EPS_Occupied)
	{
		float Distance = GetDistanceTo(CombatTarget);
		if (Distance <= AttackRange)
		{
			FVector Direction = CombatTarget->GetActorLocation() - GetActorLocation();
			Direction.Z = 0.0f;
			FRotator LookAtRotation = Direction.Rotation();
			SetActorRotation(LookAtRotation);
			PerformAttack();
		}
	}
}

void AABaseEnemyCharacter::OnSeePawn(APawn* Pawn)
{
	if (Pawn && Pawn != CombatTarget)
	{
		CombatTarget = Pawn;
	}
}

void AABaseEnemyCharacter::PerformAttack()
{
	if (PawnState == EPawnState::EPS_Occupied) return;

	PawnState = EPawnState::EPS_Occupied;

	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AABaseEnemyCharacter::OnAttackMontageEnded);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, AttackMontage);
	}
}

void AABaseEnemyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	PawnState = EPawnState::EPS_InCombat;
}

void AABaseEnemyCharacter::GetHit(const FHitResult& HitResult)
{
	if (AttributeComponent && AttributeComponent->GetHealth() <= 0.0f) return;

	if (PawnState == EPawnState::EPS_Occupied)
	{
		StopAnimMontage();
	}

	PawnState = EPawnState::EPS_Hit;

	if (HitReactMontage)
	{
		PlayAnimMontage(HitReactMontage);
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AABaseEnemyCharacter::OnAttackMontageEnded);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, HitReactMontage);
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	if (AttributeComponent)
	{
		AttributeComponent->TakeDamage(25.0f);
	}
}

void AABaseEnemyCharacter::Die()
{
	PawnState = EPawnState::EPS_Dead;
	StopAnimMontage();

	UE_LOG(LogTemp, Warning, TEXT("Przeciwnik %s zgin¹³!"), *GetName());

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(5.0f);
}