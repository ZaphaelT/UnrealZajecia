// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseCharacter.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "ABasePlayerCharacter.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;


UCLASS()
class ZAJECIA_API AABasePlayerCharacter : public AABaseCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* MoveAction;

	// Dodane: akcja Look (Axis2D: X = yaw, Y = pitch)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* EquipAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* AttackAction;

	// Override to bind Enhanced Input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Enhanced input callbacks
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
};
