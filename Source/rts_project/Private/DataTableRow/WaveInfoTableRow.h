// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WaveInfoTableRow.generated.h"


// 적 정보 로우
USTRUCT(BlueprintType)
struct FWaveEnemyElement {
	GENERATED_BODY()

public:
	// 적 코드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EnemyCode;

	// 적 스폰 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCount;

	// 적 처치 시 얻는 골드 양
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyReword;
};


// 웨이브 정보
USTRUCT(BlueprintType)
struct FWaveInfoTableRow final : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWaveEnemyElement> SpwanEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGiveWaveClearReword;

	// 아이템 리워드 코드값
	// 01 : 아이템 코드
	// 02 : 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> WaveClearItemCodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClearLimitTime;
};
