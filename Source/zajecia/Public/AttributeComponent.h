#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZAJECIA_API UAttributeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAttributeComponent();

    UPROPERTY(BlueprintAssignable, Category = "Attributes")
    FOnDeathDelegate OnDeath;

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void TakeDamage(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void SetHealth(float NewHealth);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    float MaxHealth;

private:
    UPROPERTY(VisibleAnywhere, Category = "Attributes")
    float Health;
};