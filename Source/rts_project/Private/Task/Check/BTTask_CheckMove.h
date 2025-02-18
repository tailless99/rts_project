// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckMove.generated.h"

/**
 * 
 */
UCLASS()
class UBTTask_CheckMove : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector IsMoveSucceed;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector IsMouseMove;
	
public:
	UBTTask_CheckMove();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
