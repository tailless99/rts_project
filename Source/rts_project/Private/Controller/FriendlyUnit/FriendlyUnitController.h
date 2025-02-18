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
	// ������ ���� ������ġ Ű��
	const static FName BBKey_StartLocation;

	// ������ ��ǥ��ġ Ű��
	const static FName BBKey_TargetLocation;

	// �������� �������� üũ�ϴ� Ű��
	const static FName BBKey_IsMoveSucceed;

	// ������ �����ϴ��� üũ�ϴ� Ű��
	const static FName BBKey_IsPetroll;

	// ���ڸ����� Ȧ���Ǵ��� üũ�ϴ� Ű��
	const static FName BBKey_IsHold;

	// ���콺�� �����̴��� üũ�ϴ� Ű��
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
	
	// ��ǥ ���� �Լ�
	void SetTargetLocation(FVector startLocation, FVector targetLocation);

	void SetBehaviorConditionCheck(bool IsHold, bool IsPetroll, bool IsMouseMove);
	
	// �ൿ������ �Ǵ��ϴ� �Լ�
	bool GetIsUseBehaviorAction();

	// Ȧ�� ������ �Ǵ��ϴ� �Լ�
	bool GetIsUseHoldAction();
	
};
