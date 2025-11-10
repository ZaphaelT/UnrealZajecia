#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/BaseInteractionActor.h" 
#include "PickableWeapon.generated.h"

class UBoxComponent;
class UStaticMeshComponent; // <-- DODAJ TO

UCLASS()
class ZAJECIA_API APickableWeapon : public ABaseInteractionActor
{
    GENERATED_BODY()

public:
    APickableWeapon();

    // TO JEST NOWY G£ÓWNY KOMPONENT
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* WeaponMesh;

    // Punkt 4: Komponent kolizji (box collision)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    UBoxComponent* HitboxComponent;

public:
    virtual void PickUp(AActor* Interactor) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    USceneComponent* GripPoint;

    USceneComponent* GetGripPoint() const { return GripPoint; }

    UBoxComponent* GetHitbox() const { return HitboxComponent; }
};