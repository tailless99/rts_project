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

	// ���ʹ� ���� �����̳� ����
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WIDGET_COMP"));
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	if (WIDGETBP_ENEMYHPPERCENTBAR.Succeeded())
	{
		WidgetComponent->SetWidgetClass(WIDGETBP_ENEMYHPPERCENTBAR.Class);
	}

	// �ڵ� ����
	AutoPossessAI = EAutoPossessAI::Disabled;
	// ����� ��Ʈ�ѷ� ����
	AIControllerClass = AEnemyControllerBase::StaticClass();

	// ��Ʈ�ѷ��� ���� ȸ�� ��Ȱ��ȭ
	bUseControllerRotationYaw = false;
	
	// �����Ʈ�� ���ؼ� ��Ʈ�ѷ��� ȸ���� �޾� ȸ���ϵ��� ����
	// ���� �ε巴�� ȸ���ϱ� ���� �����Ʈ�� �ϴ°�
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	// 1�ʿ� � ȸ���� �� �ִ���, �� ȸ���ӵ��� ����
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

	// �±׼���
	Tags.Empty();
	Tags.Add(TAG_ENEMY);

	// �ݸ��� ����
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
	// ���ʹ� �ڵ� üũ
	if (EnemyCode.IsNone()) return;

	// �ɷ�ġ �ʱ�ȭ
	MaxHp = CurrentHp = enemyInfo->MaxHp;
	Defensive = enemyInfo->Defensive;
	Speed = enemyInfo->Speed;
	Reward = enemyInfo->Reward;

	// �ȱ� �ӵ� ����
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
	
	// �ִ� �ν��Ͻ� ���ǵ� �ʱ�ȭ
	UEnemyAnimInstance* EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	
	if(EnemyAnimInstance)
		EnemyAnimInstance->Speed = 100.f;
}

void AEnemyCharacterBase::EnemyStunLogin(float stunTime)
{
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	
	// ���� Ÿ�̸Ӱ� �̹� �����ִٸ� ��ȯ
	if (timerManager.IsTimerActive(stunTimer))	return;
	
	// �ִ��ν��Ͻ� �ӵ� ����
	UEnemyAnimInstance* EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyAnimInstance->Speed = 0.f;

	// �ִ� �̵��ӵ��� 0���� ����
	GetCharacterMovement()->MaxWalkSpeed = 0.f;

	// ���� �ð� �� �ٽ� ���� �ӵ��� �����ϵ��� Ÿ�̸� ����
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
	
	// ü�� �ۼ�Ʈ ����
	float setEnemyHpPercent = (CurrentHp / MaxHp);
	EnemyHpPercentBar->SetEnemyHpPercent(setEnemyHpPercent);

	if (CurrentHp <= 0) {
		OnEnemyDead();		
	}
	return 0.0f;
}

// ���ʹ� ����� ȣ��
void AEnemyCharacterBase::OnEnemyDead()
{
	// ž ���� ������Ʈ ��������
	AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	UTopWidgetContainer* topWidget = playerController->MainContainerWidget->GetTopWidgetContainer();

	// ���̺꿡 ī��Ʈ ����
	topWidget->UpdateEnemyCount(-1.0f);

	// �����ֱ�
	topWidget->UpdateEnemyReward(Reward);

	// ���̺꿡 ���� ����
	AGameLevelScriptActor* GameLevel = Cast<AGameLevelScriptActor>(GetWorld()->GetCurrentLevel()->GetLevelScriptActor());
	UWaveControllerComponent* waveComponent;
	
	if (IsValid(GameLevel))
	{
		waveComponent = GameLevel->GetWaveComponent();
		waveComponent->CurrentEnemyCount -= 1;
	}
	// ����
	Destroy();
}
