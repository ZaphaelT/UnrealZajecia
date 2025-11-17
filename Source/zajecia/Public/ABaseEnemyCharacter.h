// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseCharacter.h"
#include "CombatInterface.h"
#include "EnumStates/PawnStates.h"                      // Wymagane do Enuma
#include "Perception/PawnSensingComponent.h" // Wymagane do Zmys³ów (Zadanie 7)
#include "ABaseEnemyCharacter.generated.h"

class UAttributeComponent;

UCLASS()
class ZAJECIA_API AABaseEnemyCharacter : public AABaseCharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	AABaseEnemyCharacter();

	// --- ZADANIE 7: Funkcja Tick do myœlenia AI ---
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit(const FHitResult& HitResult) override;

protected:
	virtual void BeginPlay() override;

	// --- Komponenty ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributeComponent* AttributeComponent;

	// ZADANIE 7: Komponent zmys³ów
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	// --- Walka ---
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;

	// ZADANIE 7: Animacja ataku przeciwnika
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* HitSound;

	// --- AI i Stany ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EPawnState PawnState;

	// ZADANIE 7: Kogo atakujemy
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 150.0f;

	// --- Funkcje ---
	UFUNCTION()
	void Die();

	// ZADANIE 7: Funkcje AI
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	void PerformAttack();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};