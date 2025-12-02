// Fill out your copyright notice in the Description page of Project Settings.


#include "AIConp/BaseEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ABasePlayerCharacter.h" // Twój gracz

ABaseEnemyAIController::ABaseEnemyAIController()
{
	// Inicjalizacja komponentów
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	// Konfiguracja wzroku (Sight)
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 1700.0f;
	SightConfig->PeripheralVisionAngleDegrees = 60.0f;
	// Wykrywamy wszystkich (wrogów, neutralnych, przyjació³)
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ABaseEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Uruchomienie Blackboard i Behavior Tree
	if (BehaviorTree)
	{
		if (BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		}
		BehaviorTreeComp->StartTree(*BehaviorTree);
	}

	// Podpiêcie funkcji widzenia
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseEnemyAIController::OnTargetDetected);
}

void ABaseEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	// Sprawdzamy, czy to gracz (mo¿esz u¿yæ tagów lub rzutowania)
	AABasePlayerCharacter* Player = Cast<AABasePlayerCharacter>(Actor);
	if (!Player) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		// WIDZIMY GRACZA -> Zapisz do Blackboard [cite: 36, 44]
		GetBlackboardComponent()->SetValueAsObject(Key_TargetActor, Player);
		GetBlackboardComponent()->SetValueAsBool(Key_CanSeePlayer, true);
	}
	else
	{
		// ZGUBILIŒMY GRACZA -> Zapisz ostatni¹ pozycjê [cite: 45]
		GetBlackboardComponent()->SetValueAsVector(Key_LastKnownLocation, Stimulus.StimulusLocation);
		GetBlackboardComponent()->SetValueAsObject(Key_TargetActor, nullptr); // Czyœcimy cel
		GetBlackboardComponent()->SetValueAsBool(Key_CanSeePlayer, false);
	}
}
