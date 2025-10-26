// Fill out your copyright notice in the Description page of Project Settings.

#include "ABasePlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"

void AABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Enhanced Input actions
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Move);
		}
		if (EquipAction)
		{
			EIC->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Equip);
		}
		if (AttackAction)
		{
			EIC->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Attack);
		}
	}

	// Register mapping context on the local player's Enhanced Input Subsystem
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (MappingContext)
				{
					Subsystem->AddMappingContext(MappingContext, 0);
				}
			}
		}
	}
}

void AABasePlayerCharacter::Move(const FInputActionValue& Value)
{
	// Odczyt Axis2D (konwencja: X = forward, Y = right)
	FVector2D MoveVector = Value.Get<FVector2D>();

	if (!Controller) return;

	// Przytnij wektor do d³ugoœci 1
	MoveVector = MoveVector.GetClampedToMaxSize(1.0f);

	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Po³¹cz kierunek i si³ê w jeden wektor
	FVector MoveDir = ForwardDirection * MoveVector.X + RightDirection * MoveVector.Y;
	const float Strength = MoveDir.Size();
	if (Strength > KINDA_SMALL_NUMBER)
	{
		AddMovementInput(MoveDir.GetSafeNormal(), Strength);
	}
}

void AABasePlayerCharacter::Equip(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Verbose, TEXT("Equip action triggered"));
	// Tutaj wstaw logikê equip/interaction
}

void AABasePlayerCharacter::Attack(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Verbose, TEXT("Attack action triggered"));
	// Tutaj wstaw logikê ataku
}

