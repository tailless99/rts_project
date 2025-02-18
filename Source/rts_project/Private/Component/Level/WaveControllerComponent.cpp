// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Level/WaveControllerComponent.h"
#include "DataTableRow/FriendlyUnitDataTableRow.h"
#include "DataTableRow/WaveInfoTableRow.h"
#include "DataTableRow/EnemyDataTableRow.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "Actor/LocationActor/UnitSpawnLocation.h"
#include "Actor/LocationActor/WispSpawnLocation.h"
#include "Actor/LocationActor/ActorSpawnLocationBase.h"
#include "Actor/EnemyCharacter/EnemyCharacterBase.h"
#include "Actor/EnemySpawnPoint/EnemySpawnPoint.h"
#include "Controller/FriendlyUnit/FriendlyUnitController.h"
#include "Controller/Enemy/EnemyControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "rts_project/rts_project.h"
#include "Controller/PlayerCharacter/GamePlayerController.h"
#include "Widget/MainUI/Main_Container_Widget.h"
#include "Widget/MainUI/TopWidget/TopWidgetContainer.h"


const FName UWaveControllerComponent::WispUnitCode = TEXT("0003");
const int UWaveControllerComponent::MaxEnemyCount = 60;

UWaveControllerComponent::UWaveControllerComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_WAVEINFO(
		TEXT("/Script/Engine.DataTable'/Game/DataTableRow/DT_WaveDataRow.DT_WaveDataRow'"));
	
	if (DT_WAVEINFO.Succeeded())
	{
		WaveInfoDataRow = DT_WAVEINFO.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PLAYERUNITINFO(
		TEXT("/Script/Engine.DataTable'/Game/DataTableRow/DT_UnitDataRow.DT_UnitDataRow'"));

	if (DT_PLAYERUNITINFO.Succeeded())
	{
		PlayerControllUnitDataRow = DT_PLAYERUNITINFO.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ENEMYINFO(
		TEXT("/Script/Engine.DataTable'/Game/DataTableRow/DT_EnemyDataRow.DT_EnemyDataRow'"));

	if (DT_ENEMYINFO.Succeeded())
	{
		EnemyDataRow = DT_ENEMYINFO.Object;
	}


	PrimaryComponentTick.bCanEverTick = true;


}


void UWaveControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ���� �Ҵ�
	AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	TopContainerWidget = playerController->MainContainerWidget->GetTopWidgetContainer();

	// ���� ���� ī��Ʈ �ʱ�ȭ
	CurrentEnemyCount = 0;

	// ���� ���̺� ���� �ʱ�ȭ
	CurrentWaveIndex = 1;

	// ����, ���� ������ġ ����
	FindLocationActor();

	// �ʱ� ���޹�ǰ
	GiveStarterPack();

	// ���̺� ����
	StartWave(CurrentWaveIndex);
}

// ���ְ� ������ ������ġ ����
void UWaveControllerComponent::FindLocationActor()
{
	for (TActorIterator<AActorSpawnLocationBase> It(GetWorld()); It; ++It) {
		AActorSpawnLocationBase* Actor = *It;

		if (AUnitSpawnLocation* unitSpawnLocation = Cast<AUnitSpawnLocation>(Actor)) {
			UnitSpawnLocation = unitSpawnLocation->GetActorLocation();
		}
		else if(AWispSpawnLocation* wispSpawnLocation = Cast<AWispSpawnLocation>(Actor)) {
			WispSpawnLocation = wispSpawnLocation->GetActorLocation();
		}
	}

	for (TActorIterator<AEnemySpawnPoint> It(GetWorld()); It; ++It)
	{
		AEnemySpawnPoint* Actor = *It;
		if (FName(Actor->GetActorNameOrLabel()) == TEXT("BP_EnemySpawnPoint1"))
			EnemySpawnLocation = Actor->GetActorLocation();
	}
}


void UWaveControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// �ʵ��� �� ������ �ִ� ���ڸ� �Ѿ��� ��� �й�
	if (CurrentEnemyCount >= MaxEnemyCount)
	{
		AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
		playerController->MainContainerWidget->GameOver();
	}
}

// ���� ��ȯ ����
void UWaveControllerComponent::SpawnPlayerControllUnit(FFriendlyUnitDataTableRow* playerControllUnitInfo, FName unitCode, FVector spawnLocation)
{
	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// �÷��̾ ������ �� �ִ� ������ ��ȯ
	AFriendlyAbleCharacterBase* PlayerControllUnit = GetWorld()->SpawnActor<AFriendlyAbleCharacterBase>(
		playerControllUnitInfo->UseCharacterClass,
		spawnLocation,
		FRotator::ZeroRotator,
		spawnParam);
		
	// ��Ʋ�ѷ� ���� ����
	AFriendlyUnitController* unitController = GetWorld()->SpawnActor<AFriendlyUnitController>(
		playerControllUnitInfo->UseControllerClass,
		spawnLocation,
		FRotator::ZeroRotator);

	// ĳ���� �ʱ�ȭ
	PlayerControllUnit->InitializeUnitBase(
		playerControllUnitInfo,
		unitController,
		unitCode);

	// ������ ĳ���͸� ��Ʋ�ѷ��� ����
	unitController->OnPossess(PlayerControllUnit);

	// ĳ������ �ൿ�� ����
	unitController->RunBehaviorTree(playerControllUnitInfo->UseBehavior);
}

// �ܺο��� ����ϴ� �Ʊ����� ��ȯ ��û �Լ�
void UWaveControllerComponent::CallbackRequest_SpawnUnit(FName unitCode)
{
	FString contextString;
	FFriendlyUnitDataTableRow* playerControllUnitInfo = PlayerControllUnitDataRow->FindRow<FFriendlyUnitDataTableRow>(unitCode, contextString);
	SpawnPlayerControllUnit(
		playerControllUnitInfo,
		unitCode,
		UnitSpawnLocation);
}

// �� ��ȯ
void UWaveControllerComponent::SpawnEnemyUnit(FEnemyDataTableRow* enemyInfo)
{
	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// �� ������ ��ȯ
	AEnemyCharacterBase* EnemyUnit = GetWorld()->SpawnActor<AEnemyCharacterBase>(
		enemyInfo->UseCharacterClass,
		EnemySpawnLocation,
		FRotator(0.f, 180.f, 0.f),
		spawnParam);

	// ��Ʋ�ѷ� ���� ����
	AEnemyControllerBase* enemyController = GetWorld()->SpawnActor<AEnemyControllerBase>(
		enemyInfo->UseControllerClass,
		EnemySpawnLocation,
		FRotator(0.f, 180.f, 0.f));

	// ĳ���� �ʱ�ȭ
	EnemyUnit->InitializeEnemyInfo(
		enemyController,
		enemyInfo);


	// ������ ĳ���͸� ��Ʋ�ѷ��� ����
	enemyController->OnPossess(EnemyUnit);
	enemyController->EnemyRunBehaviorTree(enemyInfo);
}

// ���� ���� ����
void UWaveControllerComponent::CreateWispUnit(int32 spwanCount)
{
	// ���� ���� ��������
	FString contextString;	
	FFriendlyUnitDataTableRow* playerControllUnitInfo1 = PlayerControllUnitDataRow->FindRow<FFriendlyUnitDataTableRow>(WispUnitCode, contextString);

	// ������ ������ ���� ���ڸ�ŭ ����
	for (int i = 0; i < spwanCount; i++) {
		SpawnPlayerControllUnit(
			playerControllUnitInfo1,
			WispUnitCode,
			WispSpawnLocation + FVector::RightVector * i * 50.0f);
	}
}


// ��Ÿ���� ����
void UWaveControllerComponent::GiveStarterPack()
{
	// �ʱ� �������� ����
	int32 GiveWispCount = 4;	// �ʱ� ���� ���� ����
	CreateWispUnit(GiveWispCount);

	// �ʱ� �ڱ� 10�� ����
	TopContainerWidget->UpdateEnemyReward(10.0f);
}

// ���̺� ����
void UWaveControllerComponent::StartWave(int32 waveIndex)
{
	// ���̺� ���� ��������
	FName WaveCount = FName(FString::Printf(TEXT("%04d"), waveIndex));
	FString contextString;
	FWaveInfoTableRow* waveInfo = WaveInfoDataRow->FindRow<FWaveInfoTableRow>(WaveCount, contextString);

	// ���ʹ� �����͸� ������ ����
	if (!waveInfo) return;
	FName SpawnEnemyCode = waveInfo->SpwanEnemies[0].EnemyCode;	
	WaveEnemyMaxCount = waveInfo->SpwanEnemies[0].SpawnCount;

	// ���ʹ� ����
	EnemyInfo = EnemyDataRow->FindRow<FEnemyDataTableRow>(SpawnEnemyCode, contextString);
	
	// ���� ������ ���
	TopContainerWidget->SetRound(waveIndex);

	// ������� ��ȯ�� ���� �ʱ�ȭ
	WaveEnemyCurrentCount = 0;

	// ���ʹ� ���� Ÿ�̸� ���
	GetWorld()->GetTimerManager().SetTimer(
		EnemySpawnTimer, FTimerDelegate::CreateUObject(this, &ThisClass::CALLBACK_SpawnEnemy_TimerFunc),
		1.0f, true);

	// ���̺� �ð� �Ҵ�
	WaveTimeCount = waveInfo->ClearLimitTime;
	TopContainerWidget->SetTimer(WaveTimeCount);

	// ���̺� ���� �ð�
	GetWorld()->GetTimerManager().SetTimer(
		WaveTimeTimer, FTimerDelegate::CreateUObject(this, &ThisClass::CALLBACK_WaveTime_TimerFunc),
		1.0f, true);
}


// ���ʹ̸� ��ȯ�ϴ� Ÿ�̸� �Լ�
void UWaveControllerComponent::CALLBACK_SpawnEnemy_TimerFunc()
{
	

	
	if (WaveEnemyCurrentCount < WaveEnemyMaxCount)
	{
		SpawnEnemyUnit(EnemyInfo);
		CurrentEnemyCount += 1;
		TopContainerWidget->UpdateEnemyCount(1.0f);
		WaveEnemyCurrentCount++;
	}
	else
	{ 
		GetWorld()->GetTimerManager().ClearTimer(EnemySpawnTimer);		
	}
}

// ���̺� Ŭ����
void UWaveControllerComponent::ClearWave()
{
	// ���̺� Ŭ���� ���� ����	
	CreateWispUnit(2);	// ���� ���� ����

	// ���� ���̺� ���� ����
	CurrentWaveIndex++;

	// �ִ� ���� ���� üũ
	const FString contextString;
	TArray<FWaveInfoTableRow*> AllRows;
	WaveInfoDataRow->GetAllRows<FWaveInfoTableRow>(contextString, AllRows);
	int32 waveMaxLound = AllRows.Num();
	if (CurrentWaveIndex >= waveMaxLound)
	{	// �ִ� ���忡 �������� �� ��������
		SLOG(TEXT("Game Win!"));
		AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
		playerController->MainContainerWidget->GameWin();
	}

	// ���� ���̺� ����
	StartWave(CurrentWaveIndex);
}

// ���̺� ���� �ð�
void UWaveControllerComponent::CALLBACK_WaveTime_TimerFunc()
{
	// ���̺��� ���� �ð��� 0���� Ŭ ��
	if(WaveTimeCount > 0)
	{
		WaveTimeCount--;
		TopContainerWidget->UpdateTimer();
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(WaveTimeTimer);
		ClearWave();
	}
}