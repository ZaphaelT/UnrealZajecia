// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseCharacter.h"
#include "EnumStates/PawnStates.h"     
// Nie musimy tu includowaæ "HUD/MainHUD.h", wystarczy forward declaration poni¿ej
#include "ABasePlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UInteractionComponent;
class UAttributeComponent;
class UAnimMontage;
class APickableWeapon;
class UMainHUD; // <-- ZMIANA: U¿ywamy Twojej nazwy klasy
struct FInputActionValue;

UCLASS()
class ZAJECIA_API AABasePlayerCharacter : public AABaseCharacter
{
	GENERATED_BODY()
public:
	AABasePlayerCharacter();

	void StartWeaponTrace();
	void EndWeaponTrace();

public:
	// --- INPUT ---
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

	// --- COMPONENTS ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	UInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	UAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	APickableWeapon* CurrentWeapon;

	// --- COMBAT & ANIMATION ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackMontage;

	void Attack(const FInputActionValue& Value);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Equip(APickableWeapon* Weapon);
	void Interact();

protected:
	void PerformAttackTrace();
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMainHUD> MainHUDClass;

	UPROPERTY()
	UMainHUD* MainHUD;

	UFUNCTION()
	void OnHealthChanged(float Current, float Max);

	UFUNCTION()
	void OnStaminaChanged(float Current, float Max);

	void SetPawnState(EPawnState NewState);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EPawnState CurrentPawnState;

private:
	bool bIsAttacking;

	UPROPERTY()
	TArray<AActor*> HitActors;
};