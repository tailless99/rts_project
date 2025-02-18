#include "Actor/UseableUnit/FriendlyCharacter/Shop/ShopActor.h"
#include "Controller/FriendlyUnit/FriendlyUnitController.h"
#include "Controller/PlayerCharacter/GamePlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Widget/MainUI/TopWidget/TopWidgetContainer.h"
#include "Widget/MainUI/Main_Container_Widget.h"
#include "Level/GameLevelScriptActor.h"
#include "Component/Level/WaveControllerComponent.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"
#include "rts_project/rts_project.h"



AShopActor::AShopActor()
{	
	static ConstructorHelpers::FObjectFinder<USoundWave> SW_COIN(
		TEXT("/Script/Engine.SoundWave'/Game/Assets/Sound/coin-sound.coin-sound'"));

	if (SW_COIN.Succeeded())
	{
		SoundWave = SW_COIN.Object;
	}

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AUDIO_COMP"));
	AudioComponent->SetupAttachment(GetRootComponent());
	AudioComponent->bAutoActivate = false;

	// 유닛 코드 설정
	UnitCode = TEXT("0006");
}

void AShopActor::BeginPlay()
{
	Super::BeginPlay();
}

void AShopActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AShopActor::SkillBtn05()
{
	Super::SkillBtn05();

	// 탑 위젯 컴포넌트 가져오기
	AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	UTopWidgetContainer* topWidget = playerController->MainContainerWidget->GetTopWidgetContainer();

	// 예외처리
	if (topWidget->GetGold() < 10) return false;

	// 랜덤으로 지급될 보상 - 비용
	float ReturnGold = FMath::RandRange(0, 20) - 10;

	// 보상 지급
	topWidget->UpdateEnemyReward(ReturnGold);

	// 초기 재생 방지를 위해 이곳에서 할당
	if (AudioComponent->GetSound() == nullptr)
		AudioComponent->SetSound(SoundWave);

	// 효과음 재생
	AudioComponent->Play();

	return true;
}

bool AShopActor::SkillBtn06()
{
	Super::SkillBtn06();

	// 탑 위젯 컴포넌트 가져오기
	AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	UTopWidgetContainer* topWidget = playerController->MainContainerWidget->GetTopWidgetContainer();

	// 예외처리
	if (topWidget->GetGold() < 50) return false;
		
	// 기본 유닛들의 코드를 랜덤으로 설정
	int32 randomUnitCode = FMath::RandRange(1, 2);
	FString FormattedString = FString::Printf(TEXT("%04d"), randomUnitCode);

	// 웨이브 컴포넌트에 전달하여 스폰 요청
	waveComponent->CallbackRequest_SpawnUnit(FName(FormattedString));

	// 비용 차감
	topWidget->UpdateEnemyReward(-50.f);

	return true;
}
