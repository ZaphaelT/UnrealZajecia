// Fill out your copyright notice in the Description page of Project Settings.


#include "AMasterGameMode.h"
#include "ABasePlayerCharacter.h"
#include "ABasePlayerController.h"

AAMasterGameMode::AAMasterGameMode()
{
	DefaultPawnClass = AABasePlayerCharacter::StaticClass();
	PlayerControllerClass = AABasePlayerController::StaticClass();
}