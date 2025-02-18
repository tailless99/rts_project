#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DetourCrowdAIController.h"
#include "FriendlyUnitController.generated.h"

/**
 * 
 */
UCLASS()
class AFriendlyUnitController : public AAIController
{
	GENERATED_BODY()
	
protected:
	// 블랙보드 정찰 시작위치 키값
	const static FName BBKey_StartLocation;

	// 블랙보드 목표위치 키값
	const static FName BBKey_TargetLocation;

	// 움직임이 끝나는지 체크하는 키값
	const static FName BBKey_IsMoveSucceed;

	// 정찰을 실행하는지 체크하는 키값
	const static FName BBKey_IsPetroll;

	// 제자리에서 홀딩되는지 체크하는 키값
	const static FName BBKey_IsHold;

	// 마우스로 움직이는지 체크하는 키값
	const static FName BBKey_IsMouseMove;

protected:
	UPROPERTY()
	class AFriendlyAbleCharacterBase* PlayerUnit;

	UPROPERTY()
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY()
	class UBehaviorTree* behaviorTreeComponent;

	UPROPERTY()
	class UDataTable* unitDataTableInfo;

private:
	AFriendlyUnitController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnPossess(APawn* pawn) override;

public:
	void UnitStopMovement();

	void RunEnemyBehaviorTree(UBehaviorTree* behaviorTree);
	
	// 목표 설정 함수
	void SetTargetLocation(FVector startLocation, FVector targetLocation);

	void SetBehaviorConditionCheck(bool IsHold, bool IsPetroll, bool IsMouseMove);
	
	// 행동중인지 판단하는 함수
	bool GetIsUseBehaviorAction();

	// 홀드 중인지 판단하는 함수
	bool GetIsUseHoldAction();
	
};
