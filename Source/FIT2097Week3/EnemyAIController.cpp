// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"


AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Configuration"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);

	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnSensesUpdated);

	TargetPlayer = nullptr;

	//Hardcoded coordinates
	FVector Waypoint1(2510, 80, 220);
	FVector Waypoint2(2510, 1310, 220);
	FVector Waypoint3(3780, 1310, 200);
	FVector Waypoint4(3780, 80, 220);

	PatrollerWaypoints.Add(Waypoint1);
	PatrollerWaypoints.Add(Waypoint2);
	PatrollerWaypoints.Add(Waypoint3);
	PatrollerWaypoints.Add(Waypoint4);

	PatrolCount = 0;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!AIBlackBoard)
		return;
	if (!ensure(BehaviorTree))
		return;

	UseBlackboard(AIBlackBoard, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);

	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	GenerateNewRandomLocation();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
	}
}

FRotator AEnemyAIController::GetControlRotation() const
{
	if (GetPawn())
	{
		return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.f);
	}

	return FRotator(0, 0, 0);
}

/* Deprecated Code
void AEnemyAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for(auto actor : DetectedPawns)
	{
		APawn* tempPawn = Cast<APawn>(actor);

		if(tempPawn)
		{
			if(tempPawn->IsPlayerControlled())
			{
				if(TargetPlayer && TargetPlayer == tempPawn)
				{
					UE_LOG(LogTemp, Warning, TEXT("Player is already known and is now leaving target space"));
					TargetPlayer = nullptr;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Player has been spotted. Saving reference"));
					TargetPlayer = tempPawn;
				}
			}
		}
	}
}
*/

void AEnemyAIController::GenerateNewRandomLocation()
{

	if (NavigationSystem)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Nav System Found"));
		FNavLocation returnLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 2000, returnLocation);
		BlackboardComponent->SetValueAsVector("PatrolPoint", returnLocation.Location);
	}
}

void AEnemyAIController::GeneratePatrolPath()
{
	
	BlackboardComponent->SetValueAsVector("PatrolPoint", PatrollerWaypoints[PatrolCount]);

	if(PatrolCount >= 3)
	{
		PatrolCount = 0;
	}else
	{
		PatrolCount++;
	}
}


void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	UE_LOG(LogTemp, Warning, TEXT("Reached Location"));

	if (NavigationSystem)
	{
		FNavLocation returnLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 2000, returnLocation);
		MoveToLocation(returnLocation.Location);
	}
}


void AEnemyAIController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	APawn* TemporaryPawn = Cast<APawn>(UpdatedActor);
	if (TemporaryPawn && TemporaryPawn->IsPlayerControlled())
	{

		if (Stimulus.WasSuccessfullySensed())
		{
			TargetPlayer = TemporaryPawn;
			BlackboardComponent->SetValueAsBool("ChasePlayer", true);
			BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
		}
		else
		{
			TargetPlayer = nullptr;
			BlackboardComponent->ClearValue("ChasePlayer");
		}
	}
}
