#include "AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	// W³¹czamy Tick dla regeneracji staminy 
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	MaxStamina = 100.0f;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	Stamina = MaxStamina;
}

// Logika regeneracji staminy 
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Stamina < MaxStamina)
	{
		float NewStamina = Stamina + (StaminaCosts.StaminaRegenRate * DeltaTime);
		Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);

		// Aktualizujemy HUD
		OnStaminaChanged.Broadcast(Stamina, MaxStamina);
	}
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
	OnHealthChanged.Broadcast(Health, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("Zdrowie teraz: %f"), Health);

	if (Health <= 0.0f)
	{
		OnDeath.Broadcast();
	}
}

// Implementacja CanPayStaminaCost 
bool UAttributeComponent::CanPayStaminaCost(float Cost) const
{
	return Stamina >= Cost;
}

// Implementacja PayStamina 
void UAttributeComponent::PayStamina(float Cost)
{
	float NewStamina = Stamina - Cost;
	Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);

	// Wywo³anie delegatu po zmianie
	OnStaminaChanged.Broadcast(Stamina, MaxStamina);
}