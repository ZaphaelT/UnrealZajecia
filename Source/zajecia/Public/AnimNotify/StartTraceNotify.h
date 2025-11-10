#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "StartTraceNotify.generated.h"

UCLASS()
class ZAJECIA_API UStartTraceNotify : public UAnimNotify
{
    GENERATED_BODY()

protected:
    // To jest funkcja, która odpali siê, gdy animacja dojdzie do tego Notify
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};