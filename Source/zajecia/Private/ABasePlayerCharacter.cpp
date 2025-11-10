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

// --- TO JEST KLUCZOWA CZÊŒÆ, KTÓREJ BRAKUJE ---
#include "Components/BoxComponent.h"         // Wymagane dla UBoxComponent
#include "Kismet/KismetSystemLibrary.h"  // Wymagane dla BoxTraceSingle
#include "Engine/HitResult.h"            // Wymagane dla FHitResult
#include "Engine/EngineTypes.h"          // Wymagane dla EDrawDebugTrace
#include "Math/Quat.h"                   // Wymagane dla FQuat
#include "Math/Color.h"                  // Wymagane dla FLinearColor

AABasePlayerCharacter::AABasePlayerCharacter()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	// W³¹czamy Tick, poniewa¿ bêdziemy go u¿ywaæ do œledzenia ataku
	PrimaryActorTick.bCanEverTick = true;
	bIsAttacking = false;
}

void AABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Jeœli bIsAttacking jest 'true', wykonuj skan co klatkê
	if (bIsAttacking)
	{
		PerformAttackTrace();
	}
}

void AABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// ... (reszta Twojej funkcji bez zmian) ...
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Enhanced Input actions
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
	// ... (Twoja funkcja bez zmian) ...
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

void AABasePlayerCharacter::Look(const FInputActionValue& Value)
{
	// ... (Twoja funkcja bez zmian) ...
		// Odczyt Axis2D: X = yaw (poziom), Y = pitch (pion). Odwróæ Y jeœli trzeba.
	const FVector2D Look = Value.Get<FVector2D>();
	if (!Controller) return;

	// Dodaje rotacjê kontrolera/pawna
	AddControllerYawInput(Look.X);
	// Negujemy Y, by odwróciæ ruch góra/dó³
	AddControllerPitchInput(-Look.Y);
}

void AABasePlayerCharacter::Interact()
{
	// ... (Twoja funkcja bez zmian) ...
	if (InteractionComponent)
	{
		InteractionComponent->TryInteract(this);
	}
}

void AABasePlayerCharacter::Equip(APickableWeapon* Weapon)
{
	// ... (Twoja funkcja bez zmian) ...
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
	// ... (Twoja funkcja bez zmian) ...
	UE_LOG(LogTemp, Warning, TEXT("Attack triggered!"));
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
}

// NOWE FUNKCJE Z ZADANIA

// Ta funkcja bêdzie wywo³ana przez AnimNotify (Punkt 3)
void AABasePlayerCharacter::StartWeaponTrace()
{
	bIsAttacking = true;
	HitActors.Empty(); // Czyœcimy listê trafionych na pocz¹tku ka¿dego ataku
}

// Ta funkcja bêdzie wywo³ana przez AnimNotify (Punkt 3)
void AABasePlayerCharacter::EndWeaponTrace()
{
	bIsAttacking = false;
}

// G³ówna logika z zadania (Punkt 5)
void AABasePlayerCharacter::PerformAttackTrace()
{
	if (!CurrentWeapon || !CurrentWeapon->GetHitbox())
	{
		return; // Nie mamy broni albo broñ nie ma hitboxa
	}

	UBoxComponent* Hitbox = CurrentWeapon->GetHitbox();

	// Pobieramy dane z komponentu Hitbox na broni (Punkt 4)
	FVector Start = Hitbox->GetComponentLocation();
	FVector End = Start; // Skanujemy w miejscu, wiêc Start i End s¹ te same
	FVector HalfSize = Hitbox->GetScaledBoxExtent();
	FRotator Orientation = Hitbox->GetComponentRotation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this); // Ignoruj sam¹ postaæ
	ActorsToIgnore.Add(CurrentWeapon); // Ignoruj sam¹ broñ

	FHitResult HitResult;

	bool bHit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		Start,
		End,
		HalfSize,
		Orientation,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), // Skanuj kana³ 'Visibility'
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration, // RYSOWANIE DEBUGOWEGO BOXA!
		HitResult,
		true,
		FLinearColor::Red,     // Kolor gdy nie trafia
		FLinearColor::Green,   // Kolor gdy trafia
		0.1f                   // Czas rysowania
	);

	if (bHit)
	{
		// Sprawdzamy, czy ju¿ nie trafiliœmy tego aktora w tym machniêciu
		if (!HitActors.Contains(HitResult.GetActor()))
		{
			HitActors.Add(HitResult.GetActor()); // Dodaj do listy trafionych

			// ZDERZENIE!
			FVector HitLocation = HitResult.Location;
			AActor* HitActor = HitResult.GetActor();

			// 'HitLocation' to jest "miejsce w przestrzeni w którym nast¹pi³o zderzenie"
			UE_LOG(LogTemp, Warning, TEXT("Trafiono %s w miejscu: %s"), *HitActor->GetName(), *HitLocation.ToString());

			// Tutaj mo¿esz dodaæ logikê zadawania obra¿eñ, np.
			// UGameplayStatics::ApplyDamage(HitActor, 10.f, GetController(), this, UDamageType::StaticClass());
		}
	}
}