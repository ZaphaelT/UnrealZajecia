// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseCharacter.h"
#include "CombatInterface.h"
#include "ABaseEnemyCharacter.generated.h"

class UAttributeComponent;

UCLASS()
class ZAJECIA_API AABaseEnemyCharacter : public AABaseCharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	AABaseEnemyCharacter();

	virtual void GetHit(const FHitResult& HitResult) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributeComponent* AttributeComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* HitSound;

	UFUNCTION()
	void Die();
};