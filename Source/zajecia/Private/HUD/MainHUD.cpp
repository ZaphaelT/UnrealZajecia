// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/MainHUD.h"
#include "TimerManager.h"

void UMainHUD::UpdateHealth(float Current, float Max)
{
	if (HealthBar && Max > 0)
	{
		HealthBar->SetPercent(Current / Max);
	}
}

void UMainHUD::UpdateStamina(float Current, float Max)
{
	if (StaminaBar && Max > 0)
	{
		StaminaBar->SetPercent(Current / Max);
	}
}

void UMainHUD::UpdateStateText(FString StateName)
{
	if (StateText)
	{
		StateText->SetText(FText::FromString(StateName));
	}
}

void UMainHUD::UpdateEnemyHealth(float Current, float Max)
{
	if (EnemyHealthBar && Max > 0)
	{
		float Percent = Current / Max;
		EnemyHealthBar->SetPercent(Percent);
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HideEnemyHealth);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_HideEnemyHealth, this, &UMainHUD::HideEnemyHealth, 3.0f, false);
		}
	}
}

void UMainHUD::HideEnemyHealth()
{
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}