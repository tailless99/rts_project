// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTableRow/FriendlyUnitDataTableRow.h"
#include "WaveControllerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWaveControllerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UDataTable* WaveInfoDataRow;

	UPROPERTY()
	class UDataTable* PlayerControllUnitDataRow;

	UPROPERTY()
	class UDataTable* EnemyDataRow;

	UPROPERTY()
	FVector UnitSpawnLocation;

	UPROPERTY()
	FVector WispSpawnLocation;

	UPROPERTY()
	class UTopWidgetContainer* TopContainerWidget;

	struct FEnemyDataTableRow* EnemyInfo;

	// 랜덤 위습 유닛 코드
	const static FName WispUnitCode;

	// 최대 등장할 수 있는 에너미 카운트
	const static int32 MaxEnemyCount;
	
	// 웨이브에서 소환할 최대 에너미 숫자
	int32 WaveEnemyMaxCount;
		
	// 웨이브 시간
	float WaveTimeCount;

	// 적이 출현할 위치
	FVector EnemySpawnLocation;

public:
	// 현재 필드에 있는 에너미 카운트
	int32 CurrentEnemyCount;

	// 웨이브에서 현재까지 소환한 에너미 숫자
	int32 WaveEnemyCurrentCount;

	// 현재 웨이브 라운드
	int32 CurrentWaveIndex;

protected:
	FTimerHandle EnemySpawnTimer;
	FTimerHandle WaveTimeTimer;

public:	
	UWaveControllerComponent();

protected:
	virtual void BeginPlay() override;

	// 유닛, 위습의 소환 위치를 찾는 함수
	void FindLocationActor();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 랜덤위습을 생성한다
	void CreateWispUnit(int32 spwanCount);

	// 다음 웨이브까지 남은 시간을 표시하는 함수
	void CALLBACK_WaveTime_TimerFunc();

	// 스폰에너미에 사용할 타이머용 변수
	void CALLBACK_SpawnEnemy_TimerFunc();

	// 유저가 컨트롤하는 유닛을 소환한다
	void SpawnPlayerControllUnit(FFriendlyUnitDataTableRow* playerControllUnitInfo, FName unitCode, FVector spawnLocation);

	// 외부에서 스폰요청을 받는 함수
	void CallbackRequest_SpawnUnit(FName unitCode);

	// 에너미 소환
	void SpawnEnemyUnit(struct FEnemyDataTableRow* enemyInfo);

	// 게임 시작 시 지급되는 아이템
	void GiveStarterPack();

	// 웨이브 시작
	void StartWave(int32 waveIndex);

	// 웨이브 클리어
	void ClearWave();
};
