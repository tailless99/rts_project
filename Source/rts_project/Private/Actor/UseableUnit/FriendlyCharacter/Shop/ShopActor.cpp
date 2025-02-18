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

	// ���� �ڵ� ����
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

	// ž ���� ������Ʈ ��������
	AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	UTopWidgetContainer* topWidget = playerController->MainContainerWidget->GetTopWidgetContainer();

	// ����ó��
	if (topWidget->GetGold() < 10) return false;

	// �������� ���޵� ���� - ���
	float ReturnGold = FMath::RandRange(0, 20) - 10;

	// ���� ����
	topWidget->UpdateEnemyReward(ReturnGold);

	// �ʱ� ��� ������ ���� �̰����� �Ҵ�
	if (AudioComponent->GetSound() == nullptr)
		AudioComponent->SetSound(SoundWave);

	// ȿ���� ���
	AudioComponent->Play();

	return true;
}

bool AShopActor::SkillBtn06()
{
	Super::SkillBtn06();

	// ž ���� ������Ʈ ��������
	AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	UTopWidgetContainer* topWidget = playerController->MainContainerWidget->GetTopWidgetContainer();

	// ����ó��
	if (topWidget->GetGold() < 50) return false;
		
	// �⺻ ���ֵ��� �ڵ带 �������� ����
	int32 randomUnitCode = FMath::RandRange(1, 2);
	FString FormattedString = FString::Printf(TEXT("%04d"), randomUnitCode);

	// ���̺� ������Ʈ�� �����Ͽ� ���� ��û
	waveComponent->CallbackRequest_SpawnUnit(FName(FormattedString));

	// ��� ����
	topWidget->UpdateEnemyReward(-50.f);

	return true;
}
