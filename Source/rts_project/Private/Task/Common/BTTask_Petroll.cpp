// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Common/BTTask_Petroll.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "rts_project/rts_project.h"


const FName UBTTask_Petroll::IsPetroll = TEXT("IsPetroll");
const FName UBTTask_Petroll::SelfActor = TEXT("SelfActor");


UBTTask_Petroll::UBTTask_Petroll()
{
	TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetLocationKey));
	
}


EBTNodeResult::Type UBTTask_Petroll::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* bbComp = OwnerComp.GetBlackboardComponent();

	// ��Ʈ�� ������̶�� ����
	if (bbComp->GetValueAsBool(IsPetroll)) {
		PetrollLoop(bbComp);
	}

	// �۾��� ���������� �Ϸ�Ǿ��ٰ� �˸�
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}


void UBTTask_Petroll::PetrollLoop(UBlackboardComponent* bbComp)
{
	AActor* actor = Cast<AActor>(bbComp->GetValueAsObject(SelfActor));
		
	FVector CurrentLocation = actor->GetActorLocation();
	FVector destination = bbComp->GetValueAsVector(TargetLocationKey.SelectedKeyName);
	float distance = FVector::Distance(CurrentLocation, destination);
	
	// �������� �����ߴٸ�
	if (distance < 500.0f)
	{
		FVector TempVector = bbComp->GetValueAsVector(StartLocationKey.SelectedKeyName);

		// ���� ������ ���������� �ٲ۴�
		FVector StartLocation = bbComp->GetValueAsVector(TargetLocationKey.SelectedKeyName);
		bbComp->SetValueAsVector(StartLocationKey.SelectedKeyName, StartLocation);
		bbComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, TempVector);
	}
}
