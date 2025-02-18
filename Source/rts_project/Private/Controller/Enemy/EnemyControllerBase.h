// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DetourCrowdAIController.h"
#include "EnemyControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class AEnemyControllerBase : public AAIController
{
	GENERATED_BODY()
	
protected:
	// 블랙보드 목표위치 키값
	const static FName BBKey_TargetLocation1;
	const static FName BBKey_TargetLocation2;
	const static FName BBKey_TargetLocation3;
	const static FName BBKey_TargetLocation4;
	
	// 목표위치 액터 이름
	const static FName SpawnPoint1;
	const static FName SpawnPoint2;
	const static FName SpawnPoint3;
	const static FName SpawnPoint4;

public:
	UPROPERTY()
	FName EnemyCode;
		
	struct FEnemyDataTableRow* EnemyInfo;

	UPROPERTY()
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY()
	class UBehaviorTree* BehaviorTreeComponent;
		
	TArray<class AEnemySpawnPoint*> MovePoints;

	

public:
	AEnemyControllerBase();

public:
	virtual void OnPossess(APawn* pawn) override;

	void OnEnemyInfoInitialized();
	void SelectMovePoints();
	void EnemyRunBehaviorTree(FEnemyDataTableRow* enemyInfo);
};
