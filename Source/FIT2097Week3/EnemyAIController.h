// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class FIT2097WEEK3_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()


public:
	AEnemyAIController();


	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	//Generates random points in the map for the agent to navigate towards
	void GenerateNewRandomLocation();

	//Loads preset coordinates for the patroller to navigate to
	void GeneratePatrolPath();
	
	//Other method to senses updated
	//UFUNCTION()
		//void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	UFUNCTION()
		void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);

	//Variables

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI);
	float SightRadius = 500.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI);
	float SightAge = 3.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI);
	float LoseSightRadius = SightRadius + 30.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI);
	float FieldOfView = 45.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI);
	UAISenseConfig_Sight* SightConfiguration;

	UNavigationSystemV1* NavigationSystem;
	APawn* TargetPlayer;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBlackboardData* AIBlackBoard;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBlackboardComponent* BlackboardComponent;

	//Patroller specific variable
	TArray<FVector> PatrollerWaypoints;
	int PatrolCount;
};
