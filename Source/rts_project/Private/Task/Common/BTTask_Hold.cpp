// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Common/BTTask_Hold.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"


EBTNodeResult::Type UBTTask_Hold::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/*AFriendlyAbleCharacterBase* playerUnit = Cast<AFriendlyAbleCharacterBase>(OwnerComp.GetOwner());
	if(playerUnit)
		playerUnit->IsUseAction = false;*/

	return EBTNodeResult::Type();
}
