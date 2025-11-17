#include "AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    MaxHealth = 100.0f; // Domyœlna wartoœæ
}

void UAttributeComponent::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;
}

void UAttributeComponent::TakeDamage(float DamageAmount)
{
    if (Health <= 0.0f)
    {
        return;
    }

    const float NewHealth = Health - DamageAmount;
    SetHealth(NewHealth);
}

void UAttributeComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

    UE_LOG(LogTemp, Warning, TEXT("Zdrowie teraz: %f"), Health);

    if (Health <= 0.0f)
    {
        OnDeath.Broadcast();
    }
}