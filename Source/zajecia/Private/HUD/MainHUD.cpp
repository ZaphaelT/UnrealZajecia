// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainHUD.h"

void UMainHUD::UpdateHealth(float Current, float Max)
{
	if (HealthBar && Max > 0)
	{
		// ProgressBar przyjmuje wartoœæ od 0.0 do 1.0
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