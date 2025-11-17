// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MainHUD.generated.h"

/**
 * Klasa HUD obs³uguj¹ca pasek ¿ycia i staminy
 */
UCLASS()
class ZAJECIA_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// Funkcje aktualizuj¹ce (Zadanie 4, pkt 2)
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealth(float Current, float Max);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateStamina(float Current, float Max);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateStateText(FString StateName);

protected:
	// Wa¿ne: Nazwy zmiennych musz¹ byæ TAKIE SAME jak nazwy elementów w Widget Blueprint
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StateText;
};