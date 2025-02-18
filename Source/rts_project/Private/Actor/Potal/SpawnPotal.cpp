#include "Actor/Potal/SpawnPotal.h"
#include "Actor/UseableUnit/FriendlyCharacter/Wisp/WispUnit.h"
#include "Level/GameLevelScriptActor.h"
#include "Component/Level/WaveControllerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Controller/PlayerCharacter/GamePlayerController.h"
#include "Widget/MainUI/BottomWidget/BottomWidgetContainer.h"
#include "Widget/MainUI/TopWidget/TopWidgetContainer.h"
#include "Widget/MainUI/Main_Container_Widget.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "rts_project/rts_project.h"
#include "Widget/Common/ActorNameWidget.h"
#include "Components/TextBlock.h"

ASpawnPotal::ASpawnPotal()
{	
	PrimaryActorTick.bCanEverTick = true;	

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAP_COMP"));
	CapsuleComponent->SetCapsuleHalfHeight(400.0f);
	CapsuleComponent->SetCapsuleRadius(150.0f);
	SetRootComponent(CapsuleComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BODY(
		TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cylinder.SM_Cylinder'"));
		
	// 바디매시 추가
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY_MESH"));
	BodyMesh->SetupAttachment(GetRootComponent());
	BodyMesh->SetRelativeScale3D(FVector(3.0f, 3.0f, 0.1f));
	BodyMesh->SetCanEverAffectNavigation(false);
	
	if (SM_BODY.Succeeded())
	{
		BodyMesh->SetStaticMesh(SM_BODY.Object);
	}

	static ConstructorHelpers::FClassFinder<UActorNameWidget> WIDGETBP_ACTORNAME(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/Comon/WidgetBP_ActorName.WidgetBP_ActorName_C'"));
	
	// 이름 표기 위젯 추가
	ActorNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget_ActorName"));
	ActorNameWidget->SetupAttachment(GetRootComponent());
	ActorNameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	if (WIDGETBP_ACTORNAME.Succeeded())
	{
		ActorNameWidget->SetWidgetClass(WIDGETBP_ACTORNAME.Class);
	}	
	ActorNameWidget->SetVisibility(true);


}

void ASpawnPotal::BeginPlay()
{
	Super::BeginPlay();	
	
	BottomWidgetContainer = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController())
		->MainContainerWidget->GetBottomWidgetContainer();

	TopWidgetContainer = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController())
		->MainContainerWidget->GetTopWidgetContainer();

	BodyMesh->SetCollisionProfileName(COLLISIONNAME_NOCOLLISION);
	CapsuleComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::CALLBACK_OnBeginOverlap);

	UActorNameWidget* WidgetActorName = Cast<UActorNameWidget>(ActorNameWidget->GetWidget());
	WidgetActorName->InitializeWidget(ActorName);
}

void ASpawnPotal::SpwanFriendlyUnit(ESpawnType spawnType)
{
	// 웨이브 컴포넌트 가져오기
	if (!IsValid(waveComponent) || waveComponent == nullptr) {
		AGameLevelScriptActor* GameLevel = Cast<AGameLevelScriptActor>(GetWorld()->GetCurrentLevel()->GetLevelScriptActor());
		if (IsValid(GameLevel))
			waveComponent = GameLevel->GetWaveComponent();
		else  
			SLOG(TEXT("GameLevelSciptAtor Access is Failed!")); 
	}

	// 스폰 타입에 따라 유닛 스폰 요청
	switch (spawnType)
	{
		case ESpawnType::RandomUnit:
			if (true) {
				// 기본 유닛들의 코드가 0001 ~ 0006까지이기 때문에 해당 범위내의 랜덤한 유닛 코드를 저장
				int32 randomUnitCode = FMath::RandRange(1, 2);
				FString FormattedString = FString::Printf(TEXT("%04d"), randomUnitCode);

				// 웨이브 컴포넌트에 전달하여 스폰 요청
				waveComponent->CallbackRequest_SpawnUnit(FName(FormattedString));
			}
			break;

		case ESpawnType::Money:
			TopWidgetContainer->UpdateEnemyReward(1);
			break;
		case ESpawnType::Wood:
			TopWidgetContainer->UpdateWaveReward(1);
			break;
		case ESpawnType::UncommonUnit_1:
			SLOG(TEXT("UncommonUnit_1"));
			break;
		case ESpawnType::UncommonUnit_2:
			break;
		case ESpawnType::UncommonUnit_3:
			break;
		case ESpawnType::UncommonUnit_4:
			break;
		case ESpawnType::UncommonUnit_5:
			break;
		case ESpawnType::UncommonUnit_6:
			break;
		default:
			break;
	}
}

void ASpawnPotal::CALLBACK_OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TAG_WISP))
	{		
		AWispUnit* wisp = Cast<AWispUnit>(OtherActor);
		// UI 정보 업데이트
		BottomWidgetContainer->RemoveArrayAt(wisp);
		// 위습 파괴
		wisp->DestroyWisp();

		// 포탈 종류에 따른 유닛 소환
		SpwanFriendlyUnit(SpawnType);
	}
}

