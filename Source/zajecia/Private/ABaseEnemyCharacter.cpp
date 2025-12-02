// Fill out your copyright notice in the Description page of Project Settings.

#include "ABaseEnemyCharacter.h"
#include "Characters/PickableWeapon.h"
#include "AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" // Potrzebne do ustawien ruchu

AABaseEnemyCharacter::AABaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));

	// USUNIÊTO: Inicjalizacjê PawnSensingComp (zrobi to AIController w Zadaniu 5)

	PawnState = EPawnState::EPS_Idle;

	// --- USTAWIENIA DLA P£YNNEGO RUCHU (Zeby nie "slizgal sie") ---
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void AABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AttributeComponent)
	{
		AttributeComponent->OnDeath.AddDynamic(this, &AABaseEnemyCharacter::Die);
	}

	// Spawnowanie broni
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

// --- TUTAJ JEST KLUCZ DO P£YNNEGO RUCHU ---
void AABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. Jeœli martwy lub obrywa -> nic nie rób
	if (PawnState == EPawnState::EPS_Dead || PawnState == EPawnState::EPS_Hit) return;

	// 2. TYLKO JEŒLI ATAKUJE (stoj¹c w miejscu) -> OBRACAJ SIÊ DO GRACZA
	// W przeciwnym razie (gdy goni) Behavior Tree samo zajmie siê ruchem i obrotem.
	if (PawnState == EPawnState::EPS_Occupied && CombatTarget)
	{
		FVector Direction = CombatTarget->GetActorLocation() - GetActorLocation();
		Direction.Z = 0.0f;
		FRotator LookAtRotation = Direction.Rotation();

		// P³ynny obrót podczas ataku
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), LookAtRotation, DeltaTime, 5.0f));
	}
}

// Funkcja wywo³ywana przez Task w Behavior Tree
void AABaseEnemyCharacter::PerformAttack()
{
	// Zabezpieczenie: jeœli ju¿ atakujemy, nie przerywaj
	if (PawnState == EPawnState::EPS_Occupied) return;

	// --- LOGIKA STAMINY (Zadanie 5) ---
	if (AttributeComponent)
	{
		// Pobieramy koszt (upewnij sie ze w AttributeComponent.h masz FStaminaCost z polem StaminaCost_Attack)
		float Cost = AttributeComponent->StaminaCosts.StaminaCost_Attack;

		if (!AttributeComponent->CanPayStaminaCost(Cost))
		{
			// Brak sily -> nie atakuje (AI po prostu podejdzie i bedzie patrzec, albo wroci do idle)
			return;
		}

		AttributeComponent->PayStamina(Cost);
	}
	// ----------------------------------

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
	// Po ataku wracamy do InCombat
	PawnState = EPawnState::EPS_InCombat;
}

void AABaseEnemyCharacter::GetHit(const FHitResult& HitResult)
{
	if (AttributeComponent && AttributeComponent->GetHealth() <= 0.0f) return;

	// Przerwij atak przy trafieniu
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