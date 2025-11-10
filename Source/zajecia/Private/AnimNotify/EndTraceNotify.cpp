// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/EndTraceNotify.h"
#include "ABasePlayerCharacter.h" 

void UEndTraceNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    UE_LOG(LogTemp, Warning, TEXT("--- EndTraceNotify Koniec wykontywania notifaja ---"));
    if (AActor* Owner = MeshComp->GetOwner())
    {
        if (AABasePlayerCharacter* Character = Cast<AABasePlayerCharacter>(Owner))
        {
            Character->EndWeaponTrace();
        }
    }
}
