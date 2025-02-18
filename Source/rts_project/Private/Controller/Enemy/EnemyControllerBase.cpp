// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Enemy/EnemyControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DataTable.h"
#include "EngineUtils.h"
#include "DataTableRow/EnemyDataTableRow.h"
#include "rts_project/rts_project.h"
#include "Actor/EnemySpawnPoint/EnemySpawnPoint.h"
#include "Actor/EnemyCharacter/EnemyCharacterBase.h"


const FName AEnemyControllerBase::BBKey_TargetLocation1 = TEXT("MovePoint1");
const FName AEnemyControllerBase::BBKey_TargetLocation2 = TEXT("MovePoint2");
const FName AEnemyControllerBase::BBKey_TargetLocation3 = TEXT("MovePoint3");
const FName AEnemyControllerBase::BBKey_TargetLocation4 = TEXT("MovePoint4");
const FName AEnemyControllerBase::SpawnPoint1 = TEXT("BP_EnemySpawnPoint1");
const FName AEnemyControllerBase::SpawnPoint2 = TEXT("BP_EnemySpawnPoint2");
const FName AEnemyControllerBase::SpawnPoint3 = TEXT("BP_EnemySpawnPoint3");
const FName AEnemyControllerBase::SpawnPoint4 = TEXT("BP_EnemySpawnPoint4");

AEnemyControllerBase::AEnemyControllerBase()
{    
    MovePoints.Empty();
    MovePoints.SetNum(4);
}

void AEnemyControllerBase::OnEnemyInfoInitialized()
{
}

// 목표 액터 순서에맞게 리스트에 추가
void AEnemyControllerBase::SelectMovePoints()
{
    for(TActorIterator<AEnemySpawnPoint> It(GetWorld()); It; ++It)
    {
        AEnemySpawnPoint* Actor = *It;

        if (FName(Actor->GetActorNameOrLabel()) == SpawnPoint2) {
            MovePoints[0] = Actor;
        }
        if (FName(Actor->GetActorNameOrLabel()) == SpawnPoint3) {
            MovePoints[1] = Actor;
        }
        if (FName(Actor->GetActorNameOrLabel()) == SpawnPoint4) {
            MovePoints[2] = Actor;
        }
        if (FName(Actor->GetActorNameOrLabel()) == SpawnPoint1) {            
            MovePoints[3] = Actor;
        }
    }
}

void AEnemyControllerBase::EnemyRunBehaviorTree(FEnemyDataTableRow* enemyInfo)
{
    UBehaviorTree* behaviortree = enemyInfo->BehaviorTreeAsset;

    // 목표 액터 순서에맞게 리스트에 추가
    SelectMovePoints();
    
    if (IsValid(behaviortree))
    {
        RunBehaviorTree(behaviortree);

        BlackboardComponent = GetBlackboardComponent();

        BlackboardComponent->SetValueAsVector(BBKey_TargetLocation1, MovePoints[0]->GetActorLocation());
        BlackboardComponent->SetValueAsVector(BBKey_TargetLocation2, MovePoints[1]->GetActorLocation());
        BlackboardComponent->SetValueAsVector(BBKey_TargetLocation3, MovePoints[2]->GetActorLocation());
        BlackboardComponent->SetValueAsVector(BBKey_TargetLocation4, MovePoints[3]->GetActorLocation());
    }
}

void AEnemyControllerBase::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);     
}
