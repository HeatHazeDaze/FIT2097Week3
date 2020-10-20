// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnemyAIController.h"
#include "GeneratePatrolPath.generated.h"

/**
 * 
 */
UCLASS()
class FIT2097WEEK3_API UGeneratePatrolPath : public UBTTaskNode
{
	GENERATED_BODY()

		EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
