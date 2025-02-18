#include "Actor/EnemyCharacter/EnemyCharacterBase.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "Controller/Enemy/EnemyControllerBase.h"
#include "Controller/PlayerCharacter/GamePlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Component/Level/WaveControllerComponent.h"
#include "Components/WidgetComponent.h"
#include "Level/GameLevelScriptActor.h"
#include "Anim/EnemyAnimInstance.h"
#include "DataTableRow/EnemyDataTableRow.h"
#include "rts_project/rts_project.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widget/MainUI/TopWidget/TopWidgetContainer.h"
#include "Widget/MainUI/Main_Container_Widget.h"
#include "Widget/Enemy/EnemyHpPercentBarWidget.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	static ConstructorHelpers::FClassFinder<UEnemyAnimInstance> ANIMBP_ENEMYANIM(
		TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Animation/AinmBP_ENemy.AinmBP_ENemy_C'"));

	if (ANIMBP_ENEMYANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIMBP_ENEMYANIM.Class);
	}

	PrimaryActorTick.bCanEverTick = true;
	
	BodyComponent = GetCapsuleComponent();
	SetRootComponent(BodyComponent);

	static ConstructorHelpers::FClassFinder<UEnemyHpPercentBarWidget> WIDGETBP_ENEMYHPPERCENTBAR(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/Enemy/WidgetBP_EnemyHp.WidgetBP_EnemyHp_C'"));

	// 에너미 위젯 컨테이너 설정
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WIDGET_COMP"));
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	if (WIDGETBP_ENEMYHPPERCENTBAR.Succeeded())
	{
		WidgetComponent->SetWidgetClass(WIDGETBP_ENEMYHPPERCENTBAR.Class);
	}

	// 자동 빙의
	AutoPossessAI = EAutoPossessAI::Disabled;
	// 사용할 컨트롤러 지정
	AIControllerClass = AEnemyControllerBase::StaticClass();

	// 컨트롤러에 의한 회전 비활성화
	bUseControllerRotationYaw = false;
	
	// 무브먼트를 통해서 컨트롤러의 회전을 받아 회전하도록 설정
	// 좀더 부드럽게 회전하기 위해 무브먼트로 하는것
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	// 1초에 몇도 회전할 수 있는지, 즉 회전속도를 정함
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

	// 태그설정
	Tags.Empty();
	Tags.Add(TAG_ENEMY);

	// 콜리전 설정
	BodyComponent->SetCollisionProfileName(COLLISIONNAME_BEDECTECTEDACTOR);
}


void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	EnemyHpPercentBar = Cast<UEnemyHpPercentBarWidget>(WidgetComponent->GetWidget());
	EnemyHpPercentBar->InitializeWidget();
}

void AEnemyCharacterBase::PossessedBy(AController* enemyController)
{
	Super::PossessedBy(enemyController);
}


void AEnemyCharacterBase::InitializeEnemyInfo(AEnemyControllerBase* enemyController, FEnemyDataTableRow* enemyInfo)
{
	// 에너미 코드 체크
	if (EnemyCode.IsNone()) return;

	// 능력치 초기화
	MaxHp = CurrentHp = enemyInfo->MaxHp;
	Defensive = enemyInfo->Defensive;
	Speed = enemyInfo->Speed;
	Reward = enemyInfo->Reward;

	// 걷기 속도 적용
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	if (enemyInfo != nullptr) enemyController->OnEnemyInfoInitialized();
	
}

void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacterBase::CALLBACK_EnemySpeedDownEventStarted(float newSpeed)
{
	float NewSpeed = GetCharacterMovement()->MaxWalkSpeed - newSpeed;

	if(NewSpeed > 0)
		GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AEnemyCharacterBase::CALLBACK_EnemySpeedDownEventEnded()
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	
	// 애님 인스턴스 스피드 초기화
	UEnemyAnimInstance* EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	
	if(EnemyAnimInstance)
		EnemyAnimInstance->Speed = 100.f;
}

void AEnemyCharacterBase::EnemyStunLogin(float stunTime)
{
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	
	// 스턴 타이머가 이미 돌고있다면 반환
	if (timerManager.IsTimerActive(stunTimer))	return;
	
	// 애님인스턴스 속도 설정
	UEnemyAnimInstance* EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyAnimInstance->Speed = 0.f;

	// 최대 이동속도를 0으로 고정
	GetCharacterMovement()->MaxWalkSpeed = 0.f;

	// 스턴 시간 후 다시 정상 속도로 복귀하도록 타이머 설정
	timerManager.SetTimer(
		stunTimer, 
		FTimerDelegate::CreateUObject(this, &ThisClass::CALLBACK_EnemySpeedDownEventEnded), 
		stunTime, 
		false);
}

float AEnemyCharacterBase::TakeDamage(
	float DamageAmount, 
	FDamageEvent const& DamageEvent, 
	AController* EventInstigator, 
	AActor* DamageCauser)
{
	float result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHp -= DamageAmount;
	
	// 체력 퍼센트 설정
	float setEnemyHpPercent = (CurrentHp / MaxHp);
	EnemyHpPercentBar->SetEnemyHpPercent(setEnemyHpPercent);

	if (CurrentHp <= 0) {
		OnEnemyDead();		
	}
	return 0.0f;
}

// 에너미 사망시 호출
void AEnemyCharacterBase::OnEnemyDead()
{
	// 탑 위젯 컴포넌트 가져오기
	AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	UTopWidgetContainer* topWidget = playerController->MainContainerWidget->GetTopWidgetContainer();

	// 웨이브에 카운트 감소
	topWidget->UpdateEnemyCount(-1.0f);

	// 보상주기
	topWidget->UpdateEnemyReward(Reward);

	// 웨이브에 정보 전달
	AGameLevelScriptActor* GameLevel = Cast<AGameLevelScriptActor>(GetWorld()->GetCurrentLevel()->GetLevelScriptActor());
	UWaveControllerComponent* waveComponent;
	
	if (IsValid(GameLevel))
	{
		waveComponent = GameLevel->GetWaveComponent();
		waveComponent->CurrentEnemyCount -= 1;
	}
	// 삭제
	Destroy();
}
