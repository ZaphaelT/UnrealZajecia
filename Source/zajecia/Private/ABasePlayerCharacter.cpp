// Fill out your copyright notice in the Description page of Project Settings.

#include "ABasePlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "Characters/InteractionComponent.h"
#include "Characters/PickableWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "InputMappingContext.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/HitResult.h"
#include "Engine/EngineTypes.h"
#include "Math/Quat.h"
#include "Math/Color.h"
#include "CombatInterface.h" // --- NOWOŒÆ (KROK 0) ---

AABasePlayerCharacter::AABasePlayerCharacter()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	PrimaryActorTick.bCanEverTick = true;
	bIsAttacking = false;
}

void AABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttacking)
	{
		PerformAttackTrace();
	}
}

void AABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Move);
		}
		if (LookAction)
		{
			EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Look);
		}
		if (EquipAction)
		{
			EIC->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Interact);
		}
		if (AttackAction)
		{
			EIC->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABasePlayerCharacter::Attack);
		}

	}

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
	FVector2D MoveVector = Value.Get<FVector2D>();

	if (!Controller) return;

	MoveVector = MoveVector.GetClampedToMaxSize(1.0f);

	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector MoveDir = ForwardDirection * MoveVector.X + RightDirection * MoveVector.Y;
	const float Strength = MoveDir.Size();
	if (Strength > KINDA_SMALL_NUMBER)
	{
		AddMovementInput(MoveDir.GetSafeNormal(), Strength);
	}
}

void AABasePlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Look = Value.Get<FVector2D>();
	if (!Controller) return;

	AddControllerYawInput(Look.X);
	AddControllerPitchInput(-Look.Y);
}

void AABasePlayerCharacter::Interact()
{
	if (InteractionComponent)
	{
		InteractionComponent->TryInteract(this);
	}
}

void AABasePlayerCharacter::Equip(APickableWeapon* Weapon)
{
	if (!Weapon) return;

	CurrentWeapon = Weapon;

	FName SocketName = TEXT("WeaponSocket");

	if (USceneComponent* Grip = Weapon->GetGripPoint())
	{
		Grip->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
	else
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}

	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Weapon->GetRootComponent()))
	{
		PrimComp->SetSimulatePhysics(false);
		PrimComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AABasePlayerCharacter::Attack(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack triggered!"));
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
}

void AABasePlayerCharacter::StartWeaponTrace()
{
	bIsAttacking = true;
	HitActors.Empty();
}

void AABasePlayerCharacter::EndWeaponTrace()
{
	bIsAttacking = false;
}

void AABasePlayerCharacter::PerformAttackTrace()
{
	if (!CurrentWeapon || !CurrentWeapon->GetHitbox())
	{
		return;
	}

	UBoxComponent* Hitbox = CurrentWeapon->GetHitbox();

	FVector Start = Hitbox->GetComponentLocation();
	FVector End = Start;
	FVector HalfSize = Hitbox->GetScaledBoxExtent();
	FRotator Orientation = Hitbox->GetComponentRotation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(CurrentWeapon);

	FHitResult HitResult;

	bool bHit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		Start,
		End,
		HalfSize,
		Orientation,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration, // RYSOWANIE DEBUGOWEGO BOXA!
		HitResult,
		true,
		FLinearColor::Red,     // Kolor gdy nie trafia
		FLinearColor::Green,   // Kolor gdy trafia
		0.1f
	);

	if (bHit)
	{
		if (!HitActors.Contains(HitResult.GetActor()))
		{
			HitActors.Add(HitResult.GetActor());

			FVector HitLocation = HitResult.Location;
			AActor* HitActor = HitResult.GetActor();

			UE_LOG(LogTemp, Warning, TEXT("Trafiono %s w miejscu: %s"), *HitActor->GetName(), *HitLocation.ToString());

			// --- NOWOŒÆ (KROK 0 / ZADANIE 3) ---
			// SprawdŸ, czy trafiony aktor implementuje nasz interfejs
			if (HitActor->Implements<UCombatInterface>())
			{
				// Wywo³aj funkcjê GetHit() na trafionym obiekcie
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(HitActor);
				if (CombatInterface)
				{
					// Przekazujemy HitResult, aby wróg wiedzia³, sk¹d zosta³ trafiony
					CombatInterface->GetHit(HitResult);
				}
			}
			// --- KONIEC NOWOŒCI ---
		}
	}
}