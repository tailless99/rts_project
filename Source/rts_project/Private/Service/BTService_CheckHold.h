// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckHold.generated.h"

/**
 * 
 */
UCLASS()
class UBTService_CheckHold : public UBTService
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = Blackboard)
	FBlackboardKeySelector IsHold;

public:
	UBTService_CheckHold();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;	
};
