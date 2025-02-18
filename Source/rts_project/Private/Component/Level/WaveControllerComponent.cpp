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

	// 위젯 할당
	AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	TopContainerWidget = playerController->MainContainerWidget->GetTopWidgetContainer();

	// 현재 유닛 카운트 초기화
	CurrentEnemyCount = 0;

	// 현재 웨이브 라운드 초기화
	CurrentWaveIndex = 1;

	// 유닛, 위습 스폰위치 설정
	FindLocationActor();

	// 초기 지급물품
	GiveStarterPack();

	// 웨이브 시작
	StartWave(CurrentWaveIndex);
}

// 유닛과 위습의 스폰위치 셋팅
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

	// 필드의 적 유닛이 최대 숫자를 넘었을 경우 패배
	if (CurrentEnemyCount >= MaxEnemyCount)
	{
		AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
		playerController->MainContainerWidget->GameOver();
	}
}

// 유닛 소환 로직
void UWaveControllerComponent::SpawnPlayerControllUnit(FFriendlyUnitDataTableRow* playerControllUnitInfo, FName unitCode, FVector spawnLocation)
{
	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 플레이어가 조종할 수 있는 유닛을 소환
	AFriendlyAbleCharacterBase* PlayerControllUnit = GetWorld()->SpawnActor<AFriendlyAbleCharacterBase>(
		playerControllUnitInfo->UseCharacterClass,
		spawnLocation,
		FRotator::ZeroRotator,
		spawnParam);
		
	// 컨틀롤러 액터 생성
	AFriendlyUnitController* unitController = GetWorld()->SpawnActor<AFriendlyUnitController>(
		playerControllUnitInfo->UseControllerClass,
		spawnLocation,
		FRotator::ZeroRotator);

	// 캐릭터 초기화
	PlayerControllUnit->InitializeUnitBase(
		playerControllUnitInfo,
		unitController,
		unitCode);

	// 생성된 캐릭터를 컨틀롤러에 연결
	unitController->OnPossess(PlayerControllUnit);

	// 캐릭터의 행동을 실행
	unitController->RunBehaviorTree(playerControllUnitInfo->UseBehavior);
}

// 외부에서 사용하는 아군유닛 소환 요청 함수
void UWaveControllerComponent::CallbackRequest_SpawnUnit(FName unitCode)
{
	FString contextString;
	FFriendlyUnitDataTableRow* playerControllUnitInfo = PlayerControllUnitDataRow->FindRow<FFriendlyUnitDataTableRow>(unitCode, contextString);
	SpawnPlayerControllUnit(
		playerControllUnitInfo,
		unitCode,
		UnitSpawnLocation);
}

// 적 소환
void UWaveControllerComponent::SpawnEnemyUnit(FEnemyDataTableRow* enemyInfo)
{
	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 적 유닛을 소환
	AEnemyCharacterBase* EnemyUnit = GetWorld()->SpawnActor<AEnemyCharacterBase>(
		enemyInfo->UseCharacterClass,
		EnemySpawnLocation,
		FRotator(0.f, 180.f, 0.f),
		spawnParam);

	// 컨틀롤러 액터 생성
	AEnemyControllerBase* enemyController = GetWorld()->SpawnActor<AEnemyControllerBase>(
		enemyInfo->UseControllerClass,
		EnemySpawnLocation,
		FRotator(0.f, 180.f, 0.f));

	// 캐릭터 초기화
	EnemyUnit->InitializeEnemyInfo(
		enemyController,
		enemyInfo);


	// 생성된 캐릭터를 컨틀롤러에 연결
	enemyController->OnPossess(EnemyUnit);
	enemyController->EnemyRunBehaviorTree(enemyInfo);
}

// 랜덤 위습 생성
void UWaveControllerComponent::CreateWispUnit(int32 spwanCount)
{
	// 위습 정보 가져오기
	FString contextString;	
	FFriendlyUnitDataTableRow* playerControllUnitInfo1 = PlayerControllUnitDataRow->FindRow<FFriendlyUnitDataTableRow>(WispUnitCode, contextString);

	// 위습을 정해진 지급 숫자만큼 생성
	for (int i = 0; i < spwanCount; i++) {
		SpawnPlayerControllUnit(
			playerControllUnitInfo1,
			WispUnitCode,
			WispSpawnLocation + FVector::RightVector * i * 50.0f);
	}
}


// 스타터팩 지급
void UWaveControllerComponent::GiveStarterPack()
{
	// 초기 랜덤위습 지급
	int32 GiveWispCount = 4;	// 초기 지급 위습 숫자
	CreateWispUnit(GiveWispCount);

	// 초기 자금 10원 지급
	TopContainerWidget->UpdateEnemyReward(10.0f);
}

// 웨이브 시작
void UWaveControllerComponent::StartWave(int32 waveIndex)
{
	// 웨이브 정보 가져오기
	FName WaveCount = FName(FString::Printf(TEXT("%04d"), waveIndex));
	FString contextString;
	FWaveInfoTableRow* waveInfo = WaveInfoDataRow->FindRow<FWaveInfoTableRow>(WaveCount, contextString);

	// 에너미 데이터를 변수에 저장
	if (!waveInfo) return;
	FName SpawnEnemyCode = waveInfo->SpwanEnemies[0].EnemyCode;	
	WaveEnemyMaxCount = waveInfo->SpwanEnemies[0].SpawnCount;

	// 에너미 정보
	EnemyInfo = EnemyDataRow->FindRow<FEnemyDataTableRow>(SpawnEnemyCode, contextString);
	
	// 라운드 위젯에 등록
	TopContainerWidget->SetRound(waveIndex);

	// 현재까지 소환된 숫자 초기화
	WaveEnemyCurrentCount = 0;

	// 에너미 스폰 타이머 등록
	GetWorld()->GetTimerManager().SetTimer(
		EnemySpawnTimer, FTimerDelegate::CreateUObject(this, &ThisClass::CALLBACK_SpawnEnemy_TimerFunc),
		1.0f, true);

	// 웨이브 시간 할당
	WaveTimeCount = waveInfo->ClearLimitTime;
	TopContainerWidget->SetTimer(WaveTimeCount);

	// 웨이브 남은 시간
	GetWorld()->GetTimerManager().SetTimer(
		WaveTimeTimer, FTimerDelegate::CreateUObject(this, &ThisClass::CALLBACK_WaveTime_TimerFunc),
		1.0f, true);
}


// 에너미를 소환하는 타이머 함수
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

// 웨이브 클리어
void UWaveControllerComponent::ClearWave()
{
	// 웨이브 클리어 보상 증정	
	CreateWispUnit(2);	// 랜덤 위습 생성

	// 현재 웨이브 라운드 갱신
	CurrentWaveIndex++;

	// 최대 도달 라운드 체크
	const FString contextString;
	TArray<FWaveInfoTableRow*> AllRows;
	WaveInfoDataRow->GetAllRows<FWaveInfoTableRow>(contextString, AllRows);
	int32 waveMaxLound = AllRows.Num();
	if (CurrentWaveIndex >= waveMaxLound)
	{	// 최대 라운드에 도달했을 때 게임종료
		SLOG(TEXT("Game Win!"));
		AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
		playerController->MainContainerWidget->GameWin();
	}

	// 다음 웨이브 시작
	StartWave(CurrentWaveIndex);
}

// 웨이브 남은 시간
void UWaveControllerComponent::CALLBACK_WaveTime_TimerFunc()
{
	// 웨이브의 남은 시간이 0보다 클 때
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