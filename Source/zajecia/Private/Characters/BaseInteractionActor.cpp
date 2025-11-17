// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseInteractionActor.h"

// Sets default values
ABaseInteractionActor::ABaseInteractionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseInteractionActor::Interact(AActor* Interactor)
{
	PickUp(Interactor);
}

void ABaseInteractionActor::PickUp(AActor* Interactor)
{

}

