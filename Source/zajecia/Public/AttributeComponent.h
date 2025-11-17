#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

// Delegat œmierci
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);

// Delegat do aktualizacji paska ¯ycia i Staminy w HUD (Zadanie 4, pkt 2) [cite: 10, 16]
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, float, CurrentValue, float, MaxValue);

// Struktura kosztów akcji (Zadanie 4, pkt 1) 
USTRUCT(BlueprintType)
struct FStaminaCost
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaRegenRate = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaCost_Attack = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaCost_Jump = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintCostPerSec = 5.0f; // Dodatkowe dla sprintu [cite: 22]
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZAJECIA_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributeComponent();

	// Tick wymagany do regeneracji staminy 
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// --- DELEGATY ---
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnDeathDelegate OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnStaminaChanged;

	// --- FUNKCJE HEALTH ---
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void SetHealth(float NewHealth);

	// --- FUNKCJE STAMINA (Zadanie 4, pkt 1) ---
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool CanPayStaminaCost(float Cost) const; // 

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void PayStamina(float Cost); // 

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetStamina() const { return Stamina; } // 

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxStamina() const { return MaxStamina; } // 

	// Konfiguracja kosztów
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FStaminaCost StaminaCosts;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxStamina; // [cite: 4]

private:
	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float Health;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float Stamina; // 
};