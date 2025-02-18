// Fill out your copyright notice in the Description page of Project Settings.


#include "Service/BTService_CheckHold.h"

UBTService_CheckHold::UBTService_CheckHold()
{
	IsHold.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, IsHold));
}

void UBTService_CheckHold::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
