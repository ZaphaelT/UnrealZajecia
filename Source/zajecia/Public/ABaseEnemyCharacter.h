// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseCharacter.h"
#include "CombatInterface.h"
#include "EnumStates/PawnStates.h"
#include "Perception/PawnSensingComponent.h"
#include "ABaseEnemyCharacter.generated.h"

class UAttributeComponent;
class APickableWeapon;

UCLASS()
class ZAJECIA_API AABaseEnemyCharacter : public AABaseCharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	AABaseEnemyCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit(const FHitResult& HitResult) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EPawnState PawnState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 150.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat")
	APickableWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<APickableWeapon> DefaultWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName WeaponSocketName = TEXT("WeaponSocket");

	UFUNCTION()
	void Die();

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void PerformAttack(); // Wykonanie ataku

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};