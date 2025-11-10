#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatInterface.h"
#include "DestructibleActor.generated.h"

UCLASS()
class ZAJECIA_API ADestructibleActor : public AActor, public ICombatInterface
{
    GENERATED_BODY()

public:
    ADestructibleActor();

    virtual void GetHit(const FHitResult& HitResult) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* MeshComponent;

private:
    int32 HitCount;

    UPROPERTY(EditAnywhere, Category = "Destructible")
    int32 HitsToDestroy;
};