// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WaveInfoTableRow.generated.h"


// �� ���� �ο�
USTRUCT(BlueprintType)
struct FWaveEnemyElement {
	GENERATED_BODY()

public:
	// �� �ڵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EnemyCode;

	// �� ���� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCount;

	// �� óġ �� ��� ��� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyReword;
};


// ���̺� ����
USTRUCT(BlueprintType)
struct FWaveInfoTableRow final : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWaveEnemyElement> SpwanEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGiveWaveClearReword;

	// ������ ������ �ڵ尪
	// 01 : ������ �ڵ�
	// 02 : ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> WaveClearItemCodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClearLimitTime;
};
