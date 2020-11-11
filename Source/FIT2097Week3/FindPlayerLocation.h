// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnemyAIController.h"
#include "FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class FIT2097WEEK3_API UFindPlayerLocation : public UBTTaskNode
{
	GENERATED_BODY()

	//Deprecated, does not work, intent was to add a Mr. x type character (RE2).
		//EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
