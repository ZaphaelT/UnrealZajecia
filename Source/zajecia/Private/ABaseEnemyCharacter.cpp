// Fill out your copyright notice in the Description page of Project Settings.

#include "ABaseEnemyCharacter.h"
#include "AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

AABaseEnemyCharacter::AABaseEnemyCharacter()
{
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
}

void AABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AttributeComponent)
	{
		AttributeComponent->OnDeath.AddDynamic(this, &AABaseEnemyCharacter::Die);
	}
}

void AABaseEnemyCharacter::GetHit(const FHitResult& HitResult)
{
	if (AttributeComponent && AttributeComponent->GetHealth() <= 0.0f)
	{
		return;
	}

	if (HitReactMontage)
	{
		PlayAnimMontage(HitReactMontage);
	}
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	if (AttributeComponent)
	{
		AttributeComponent->TakeDamage(25.0f);
	}
}

void AABaseEnemyCharacter::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Przeciwnik %s zgin¹³!"), *GetName());

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(5.0f);
}