#include "DestructibleActor.h"

ADestructibleActor::ADestructibleActor()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    SetRootComponent(MeshComponent);

    MeshComponent->SetSimulatePhysics(true);
    MeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));

    HitCount = 0;
    HitsToDestroy = 2;
}

void ADestructibleActor::GetHit(const FHitResult& HitResult)
{
    HitCount++;
    UE_LOG(LogTemp, Warning, TEXT("Obiekt trafiony! (%d / %d)"), HitCount, HitsToDestroy);

    if (HitCount >= HitsToDestroy)
    {
        Destroy();
    }
}