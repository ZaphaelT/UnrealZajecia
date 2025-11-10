#include "AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    MaxHealth = 100.0f; // Domyœlna wartoœæ
}

void UAttributeComponent::BeginPlay()
{
    Super::BeginPlay();
    // Ustawiamy zdrowie na maksymalne na pocz¹tku gry
    Health = MaxHealth;
}

void UAttributeComponent::TakeDamage(float DamageAmount)
{
    if (Health <= 0.0f)
    {
        return; // Ju¿ martwy
    }

    const float NewHealth = Health - DamageAmount;
    SetHealth(NewHealth);
}

void UAttributeComponent::SetHealth(float NewHealth)
{
    // U¿ywamy Clamp, aby zdrowie nie spad³o poni¿ej 0 ani nie przekroczy³o MaxHealth
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

    UE_LOG(LogTemp, Warning, TEXT("Zdrowie teraz: %f"), Health);

    // Jeœli zdrowie spad³o do zera, wywo³aj delegat
    if (Health <= 0.0f)
    {
        OnDeath.Broadcast();
    }
}