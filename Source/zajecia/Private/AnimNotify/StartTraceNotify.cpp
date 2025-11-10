// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/StartTraceNotify.h"
#include "ABasePlayerCharacter.h" // Za³¹cz plik .h swojej postaci

void UStartTraceNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    UE_LOG(LogTemp, Warning, TEXT("--- StartTraceNotify Start notifaja w animacji ---"));
    // Punkt 3: Przeka¿ informacjê do postaci
    if (AActor* Owner = MeshComp->GetOwner())
    {
        if (AABasePlayerCharacter* Character = Cast<AABasePlayerCharacter>(Owner))
        {
            // Wywo³ujemy funkcjê C++ na naszej postaci
            Character->StartWeaponTrace();
        }
    }
}
