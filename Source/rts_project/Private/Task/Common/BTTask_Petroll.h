// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Petroll.generated.h"

/**
 * 
 */
UCLASS()
class UBTTask_Petroll : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(VisibleAnywhere, Category = Blackboard)
	FBlackboardKeySelector StartLocationKey;

	// ��Ʈ�� ��� Ȯ�� ����
	const static FName IsPetroll;
	// �ڱ� �ڽ�
	const static FName SelfActor;

public:
	UBTTask_Petroll();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	void PetrollLoop(UBlackboardComponent* bbComp);
};
