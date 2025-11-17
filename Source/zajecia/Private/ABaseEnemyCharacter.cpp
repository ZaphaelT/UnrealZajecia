// Fill out your copyright notice in the Description page of Project Settings.

#include "ABaseEnemyCharacter.h"
#include "AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

AABaseEnemyCharacter::AABaseEnemyCharacter()
{
	// ZADANIE 7: W³¹czamy Tick, ¿eby AI mog³o reagowaæ
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));

	// ZADANIE 7: Konfiguracja oczu
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SightRadius = 1500.0f; // Widzi na 15 metrów
	PawnSensingComp->SetPeripheralVisionAngle(60.0f); // K¹t widzenia

	PawnState = EPawnState::EPS_Idle;
}

void AABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AttributeComponent)
	{
		AttributeComponent->OnDeath.AddDynamic(this, &AABaseEnemyCharacter::Die);
	}

	// ZADANIE 7: Podpinamy funkcjê "Widzenia"
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AABaseEnemyCharacter::OnSeePawn);
	}
}

// --- ZADANIE 7: Mózg Przeciwnika ---
void AABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. Jeœli martwy lub zajêty bólem -> nic nie rób
	if (PawnState == EPawnState::EPS_Dead || PawnState == EPawnState::EPS_Hit) return;

	// 2. Jeœli mamy cel (widzimy gracza) i nie atakujemy w tej chwili
	if (CombatTarget && PawnState != EPawnState::EPS_Occupied)
	{
		float Distance = GetDistanceTo(CombatTarget);

		// 3. Jeœli gracz jest blisko -> OBRÓÆ SIÊ I ATAKUJ
		if (Distance <= AttackRange)
		{
			// A. Obrót w stronê gracza (tylko w poziomie)
			FVector Direction = CombatTarget->GetActorLocation() - GetActorLocation();
			Direction.Z = 0.0f;
			FRotator LookAtRotation = Direction.Rotation();

			SetActorRotation(LookAtRotation);

			// B. Wykonaj Atak
			PerformAttack();
		}
	}
}

// ZADANIE 7: Co robiæ jak zobaczê gracza
void AABaseEnemyCharacter::OnSeePawn(APawn* Pawn)
{
	if (Pawn && Pawn != CombatTarget)
	{
		CombatTarget = Pawn;
		// Przeciwnik zauwa¿y³ gracza!
	}
}

// ZADANIE 7: Wykonanie ataku
void AABaseEnemyCharacter::PerformAttack()
{
	// Nie atakuj, jeœli ju¿ atakujesz
	if (PawnState == EPawnState::EPS_Occupied) return;

	PawnState = EPawnState::EPS_Occupied; // Ustaw stan na "Zajêty atakiem"

	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);

		// Wa¿ne: Musimy wiedzieæ, kiedy atak siê skoñczy, ¿eby wróciæ do normy
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AABaseEnemyCharacter::OnAttackMontageEnded);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, AttackMontage);
	}
}

void AABaseEnemyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// Atak siê skoñczy³ -> wracamy do gotowoœci
	PawnState = EPawnState::EPS_InCombat;
}

// --- Reakcja na trafienie (ZADANIE 5 i 7) ---
void AABaseEnemyCharacter::GetHit(const FHitResult& HitResult)
{
	if (AttributeComponent && AttributeComponent->GetHealth() <= 0.0f) return;

	// ZADANIE 7: Przerwij atak jeœli trafiony!
	if (PawnState == EPawnState::EPS_Occupied)
	{
		StopAnimMontage(); // Zatrzymuje animacjê ataku
	}

	PawnState = EPawnState::EPS_Hit;

	if (HitReactMontage)
	{
		PlayAnimMontage(HitReactMontage);

		// Po skoñczeniu animacji bólu te¿ wracamy do gotowoœci
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
	PawnState = EPawnState::EPS_Dead; // Ustaw stan na martwy
	StopAnimMontage(); // Zatrzymaj wszystkie animacje

	UE_LOG(LogTemp, Warning, TEXT("Przeciwnik %s zgin¹³!"), *GetName());

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(5.0f);
}