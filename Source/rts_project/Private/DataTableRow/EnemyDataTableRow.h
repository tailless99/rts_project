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
	// ����� ĳ���� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	TSubclassOf<class AEnemyCharacterBase> UseCharacterClass;

	// ���� ��Ʈ�ѷ� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AEnemyControllerBase> UseControllerClass;

	// ���� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EnemyName;

	// �ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp;

	// �̵��ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defensive;

	// �������� ������ ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Reward;

	// ����ϴ� BehaviorTree
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTreeAsset;
};
