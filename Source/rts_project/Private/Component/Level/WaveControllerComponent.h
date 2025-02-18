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

	// ���� ���� ���� �ڵ�
	const static FName WispUnitCode;

	// �ִ� ������ �� �ִ� ���ʹ� ī��Ʈ
	const static int32 MaxEnemyCount;
	
	// ���̺꿡�� ��ȯ�� �ִ� ���ʹ� ����
	int32 WaveEnemyMaxCount;
		
	// ���̺� �ð�
	float WaveTimeCount;

	// ���� ������ ��ġ
	FVector EnemySpawnLocation;

public:
	// ���� �ʵ忡 �ִ� ���ʹ� ī��Ʈ
	int32 CurrentEnemyCount;

	// ���̺꿡�� ������� ��ȯ�� ���ʹ� ����
	int32 WaveEnemyCurrentCount;

	// ���� ���̺� ����
	int32 CurrentWaveIndex;

protected:
	FTimerHandle EnemySpawnTimer;
	FTimerHandle WaveTimeTimer;

public:	
	UWaveControllerComponent();

protected:
	virtual void BeginPlay() override;

	// ����, ������ ��ȯ ��ġ�� ã�� �Լ�
	void FindLocationActor();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// ���������� �����Ѵ�
	void CreateWispUnit(int32 spwanCount);

	// ���� ���̺���� ���� �ð��� ǥ���ϴ� �Լ�
	void CALLBACK_WaveTime_TimerFunc();

	// �������ʹ̿� ����� Ÿ�̸ӿ� ����
	void CALLBACK_SpawnEnemy_TimerFunc();

	// ������ ��Ʈ���ϴ� ������ ��ȯ�Ѵ�
	void SpawnPlayerControllUnit(FFriendlyUnitDataTableRow* playerControllUnitInfo, FName unitCode, FVector spawnLocation);

	// �ܺο��� ������û�� �޴� �Լ�
	void CallbackRequest_SpawnUnit(FName unitCode);

	// ���ʹ� ��ȯ
	void SpawnEnemyUnit(struct FEnemyDataTableRow* enemyInfo);

	// ���� ���� �� ���޵Ǵ� ������
	void GiveStarterPack();

	// ���̺� ����
	void StartWave(int32 waveIndex);

	// ���̺� Ŭ����
	void ClearWave();
};
