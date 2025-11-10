#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// Ta klasa nie musi byæ modyfikowana
UINTERFACE(MinimalAPI, Blueprintable)
class UCombatInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interfejs dla wszystkich aktorów, którzy mog¹ zostaæ trafieni
 */
class ZAJECIA_API ICombatInterface
{
    GENERATED_BODY()

public:

    UFUNCTION()
    virtual void GetHit(const FHitResult& HitResult) = 0;
};