#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPawnState : uint8
{
    EPS_Idle        UMETA(DisplayName = "Idle"),
    EPS_InCombat    UMETA(DisplayName = "In Combat"),
    EPS_Occupied    UMETA(DisplayName = "Occupied (Attacking)"),
    EPS_Hit         UMETA(DisplayName = "Hit (Stunned)"),
    EPS_Dead        UMETA(DisplayName = "Dead"),
    EPS_Exhausted   UMETA(DisplayName = "Exhausted")
};
