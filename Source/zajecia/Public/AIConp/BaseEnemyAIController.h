#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BaseEnemyAIController.generated.h"

// Forward declarations
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UAIPerceptionComponent;

UCLASS()
class ZAJECIA_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	// Komponenty AI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTreeComponent* BehaviorTreeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* PerceptionComp;

	// Drzewo zachowañ do uruchomienia (przypiszemy w BP)
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	// Nazwy kluczy w Blackboard (dla wygody i unikania literówek)
	const FName Key_TargetActor = "TargetActor";
	const FName Key_LastKnownLocation = "LastKnownPlayerLocation";
	const FName Key_CanSeePlayer = "CanSeePlayer";

	// Funkcja wywo³ywana, gdy zmys³y coœ wykryj¹
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
};