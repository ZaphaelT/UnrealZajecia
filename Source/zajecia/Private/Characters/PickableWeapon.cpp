// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/PickableWeapon.h"
#include "ABasePlayerCharacter.h" 
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h" // <-- DODAJ TO

APickableWeapon::APickableWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    // 1. Stwórz StaticMesh i ustaw go jako G£ÓWNY (Root) komponent
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh); // <-- KLUCZOWA ZMIANA

    // 2. Stwórz Hitbox i PRZYCZEP GO DO MIECZA (WeaponMesh)
    HitboxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitboxComponent"));
    HitboxComponent->SetupAttachment(WeaponMesh); // <-- PRZYCZEP DO MIECZA

    // Ten box nie ma sam wykrywaæ kolizji. S³u¿y tylko jako WZÓR dla BoxTrace.
    HitboxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HitboxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

    // 3. Stwórz GripPoint i PRZYCZEP GO DO MIECZA (WeaponMesh)
    GripPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GripPoint"));
    GripPoint->SetupAttachment(WeaponMesh); // <-- PRZYCZEP DO MIECZA
}

void APickableWeapon::PickUp(AActor* Interactor)
{
    AABasePlayerCharacter* Player = Cast<AABasePlayerCharacter>(Interactor);
    if (Player)
    {
        Player->Equip(this);
    }
}