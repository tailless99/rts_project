// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Check/BTTask_CheckMove.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckMove::UBTTask_CheckMove()
{
	// 필터 추가
	IsMoveSucceed.AddBoolFilter(this, IsMoveSucceed.SelectedKeyName);
	IsMouseMove.AddBoolFilter(this, IsMouseMove.SelectedKeyName);
}

EBTNodeResult::Type UBTTask_CheckMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* bbComp = OwnerComp.GetBlackboardComponent();
	bbComp->SetValueAsBool(IsMoveSucceed.SelectedKeyName, true);
	bbComp->SetValueAsBool(IsMouseMove.SelectedKeyName, false);

	return EBTNodeResult::Type();
}
