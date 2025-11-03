// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseCharacter.h"
#include "Characters/InteractionComponent.h"
#include "InputMappingContext.h"
#include "Characters/PickableWeapon.h"
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
	AABasePlayerCharacter();

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputMappingContext* MappingContext;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    UInputAction* MoveAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    UInputAction* EquipAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    UInputAction* AttackAction;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* LookAction;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
    UInteractionComponent* InteractionComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    APickableWeapon* CurrentWeapon;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    UAnimMontage* AttackMontage;
    void Attack(const FInputActionValue& Value);

    // Override to bind Enhanced Input
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    // Enhanced input callbacks
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    virtual void Equip(APickableWeapon* Weapon);
    void Interact();
    

};
