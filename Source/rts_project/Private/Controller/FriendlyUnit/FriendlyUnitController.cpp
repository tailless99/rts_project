#include "Controller/FriendlyUnit/FriendlyUnitController.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "AnimInstance/FriendlyAbleUnitAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DataTable.h"
#include "DataTableRow/FriendlyUnitDataTableRow.h"
#include "rts_project/rts_project.h"
#include "Navigation/CrowdFollowingComponent.h"


const FName AFriendlyUnitController::BBKey_TargetLocation = TEXT("TargetLocationKey");
const FName AFriendlyUnitController::BBKey_StartLocation = TEXT("StartLocationKey");
const FName AFriendlyUnitController::BBKey_IsMoveSucceed = TEXT("IsMoveSucceed");
const FName AFriendlyUnitController::BBKey_IsPetroll = TEXT("IsPetroll");
const FName AFriendlyUnitController::BBKey_IsHold = TEXT("IsHold");
const FName AFriendlyUnitController::BBKey_IsMouseMove = TEXT("IsMouseMove");


AFriendlyUnitController::AFriendlyUnitController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DT_TEST(
        TEXT("/Script/Engine.DataTable'/Game/DataTableRow/DT_UnitDataRow.DT_UnitDataRow'"));

    if (DT_TEST.Succeeded())
    {
        unitDataTableInfo = DT_TEST.Object;

        FString contextString;
        FFriendlyUnitDataTableRow* unitInfo = unitDataTableInfo->FindRow<FFriendlyUnitDataTableRow>(FName(TEXT("0001")), contextString);
        behaviorTreeComponent = unitInfo->UseBehavior;
    }           
}

void AFriendlyUnitController::OnPossess(APawn* pawn)
{
    Super::OnPossess(pawn);

    PlayerUnit = Cast<AFriendlyAbleCharacterBase>(pawn);
    PlayerUnit->GetCharacterMovement()->bUseControllerDesiredRotation = true;
    PlayerUnit->GetCharacterMovement()->AvoidanceConsiderationRadius = 300.0f;

    RunEnemyBehaviorTree(behaviorTreeComponent);

}

void AFriendlyUnitController::Tick(float DeltaTime)
{
    // 블랙보드 컴포넌트가 유효하고, 이동이 끝났을 경우
    if (IsValid(BlackboardComponent) && Blackboard->GetValueAsBool(BBKey_IsMoveSucceed))
    {
        // 유닛 정지
        UnitStopMovement();
        // 움직임 체킹함수 초기화
        BlackboardComponent->SetValueAsBool(BBKey_IsMoveSucceed, false);
    }
}

void AFriendlyUnitController::UnitStopMovement()
{
    // 작업을 멈춤
    StopMovement();

    // 애니메이션 블랜더의 속도를 0으로 수정
    PlayerUnit->AnimInstance->OnUpdateSpeed(0.0f);
}

void AFriendlyUnitController::RunEnemyBehaviorTree(UBehaviorTree* behaviorTree)
{
    if (behaviorTree)
    {
        if (RunBehaviorTree(behaviorTree))
        {
            BlackboardComponent = GetBlackboardComponent();
        }
    }
}

void AFriendlyUnitController::SetTargetLocation(FVector startLocation, FVector targetLocation)
{
    if (IsValid(BlackboardComponent))
    {
        //StopMovement(); // 구 목적지로 움직이던 것을 멈추기 위해 일단 정지함
        
        // 애니메이션 블랜더의 속도 변경
        PlayerUnit->AnimInstance->OnUpdateSpeed(100.0f);

        // 목적지 갱신
        BlackboardComponent->SetValueAsVector(BBKey_StartLocation, startLocation);
        BlackboardComponent->SetValueAsVector(BBKey_TargetLocation, targetLocation);
        
    }
}

void AFriendlyUnitController::SetBehaviorConditionCheck(bool IsHold, bool IsPetroll, bool IsMouseMove)
{
    if (IsValid(BlackboardComponent))
    {
        BlackboardComponent->SetValueAsBool(BBKey_IsHold, IsHold);
        BlackboardComponent->SetValueAsBool(BBKey_IsPetroll, IsPetroll);
        BlackboardComponent->SetValueAsBool(BBKey_IsMouseMove, IsMouseMove);
    }
}

bool AFriendlyUnitController::GetIsUseBehaviorAction()
{
    bool CheckActioning;
    if (IsValid(BlackboardComponent))
    {
        bool a = BlackboardComponent->GetValueAsBool(BBKey_IsPetroll);
        bool b = BlackboardComponent->GetValueAsBool(BBKey_IsMouseMove);
        CheckActioning = a ? true : b ? true : false;
    }

    return CheckActioning;
}

bool AFriendlyUnitController::GetIsUseHoldAction()
{
    if(IsValid(BlackboardComponent))
        return BlackboardComponent->GetValueAsBool(BBKey_IsHold);
    
    return false;
}
