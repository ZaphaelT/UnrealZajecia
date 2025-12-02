// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseCharacter.h"
#include "CombatInterface.h"
#include "EnumStates/PawnStates.h" // Upewnij siê, ¿e œcie¿ka jest poprawna
#include "ABaseEnemyCharacter.generated.h"

class UAttributeComponent;
class APickableWeapon;

UCLASS()
class ZAJECIA_API AABaseEnemyCharacter : public AABaseCharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	AABaseEnemyCharacter();

	// Tick nie jest ju¿ potrzebny do AI, ale zostawiamy pusty w CPP na wszelki wypadek
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit(const FHitResult& HitResult) override;

	// Funkcja pomocnicza, ¿eby Task z Behavior Tree wiedzia³, czy wróg atakuje
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool IsAttacking() const { return PawnState == EPawnState::EPS_Occupied; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributeComponent* AttributeComponent;

	// USUNIÊTO: PawnSensing (teraz AIController u¿ywa AIPerception)

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EPawnState PawnState;

	// CombatTarget jest teraz trzymany w Blackboardzie, ale zostawiamy tu jako pomocnicze
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	AActor* CombatTarget;

	// Zasiêg ataku zostawiamy, przyda siê w Decoratorach BT
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AttackRange = 150.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat")
	APickableWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<APickableWeapon> DefaultWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName WeaponSocketName = TEXT("WeaponSocket");

	UFUNCTION()
	void Die();

	// USUNIÊTO: OnSeePawn (teraz AIController to obs³uguje)

public:
	// Ta funkcja ZOSTAJE - Behavior Tree bêdzie j¹ wywo³ywaæ przez Task
	UFUNCTION(BlueprintCallable, Category = "AI")
	void PerformAttack();

protected:
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};