// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratePatrolPath.h"

EBTNodeResult::Type UGeneratePatrolPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;

	if (!Component)
		return EBTNodeResult::Failed;

	AEnemyAIController* MyController = Cast<AEnemyAIController>(Component->GetOwner());

	if (!MyController)
		return EBTNodeResult::Failed;

	MyController->GeneratePatrolPath();

	return EBTNodeResult::Succeeded;
}