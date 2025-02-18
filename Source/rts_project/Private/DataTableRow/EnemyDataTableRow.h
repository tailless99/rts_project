// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemyDataTableRow.generated.h"


USTRUCT(BlueprintType)
struct FEnemyDataTableRow final : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 사용할 캐릭터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	TSubclassOf<class AEnemyCharacterBase> UseCharacterClass;

	// 사용될 컨트롤러 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AEnemyControllerBase> UseControllerClass;

	// 적의 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EnemyName;

	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp;

	// 이동속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	// 방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defensive;

	// 보상으로 제공할 골드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Reward;

	// 사용하는 BehaviorTree
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTreeAsset;
};
