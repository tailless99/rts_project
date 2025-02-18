#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "Actor/EnemyCharacter/EnemyCharacterBase.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance/FriendlyAbleUnitAnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Level/GameLevelScriptActor.h"
#include "Component/Level/WaveControllerComponent.h"
#include "Component/FriendlyUnitAttackComponent/FriendlyUnitAttackComponentBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "DataTableRow/FriendlyUnitDataTableRow.h"
#include "Controller/FriendlyUnit/FriendlyUnitController.h"
#include "Controller/PlayerCharacter/GamePlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/MainUI/Main_Container_Widget.h"
#include "Widget/MainUI/TopWidget/TopWidgetContainer.h"
#include "rts_project/rts_project.h"


AFriendlyAbleCharacterBase::AFriendlyAbleCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
		
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MT_SELECTED_EFFECT(
		TEXT("/Script/Engine.Material'/Game/Blueprints/Material/M_Effect/M_Selected_Decal_MAT.M_Selected_Decal_MAT'"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ANIM_MONTAGE(
		TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Animation/AnimMontage/Anim_SAS_Combo4_InPlace_3_Montage.Anim_SAS_Combo4_InPlace_3_Montage'"));

	if (ANIM_MONTAGE.Succeeded())
	{
		attackMontage = ANIM_MONTAGE.Object;
	}
	
	// 캡슐 콜라이더 기본 셋팅
	CapsuleCollider = GetCapsuleComponent();
	CapsuleCollider->SetCapsuleRadius(30.0f);
	CapsuleCollider->SetCapsuleHalfHeight(100.0f);
	//RootComponent = CapsuleCollider;
	SetRootComponent(CapsuleCollider);

	// 데칼 컴포넌트 설정
	SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionDecal"));
	SelectionDecal->SetupAttachment(GetRootComponent());
	SelectionDecal->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
	SelectionDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	SelectionDecal->DecalSize = FVector(15.0, 30.0, 30.0);
	SelectionDecal->SetVisibility(false);


	if (MT_SELECTED_EFFECT.Succeeded()) {
		SelectionDecal->SetDecalMaterial(MT_SELECTED_EFFECT.Object);
	}

	// 메시 설정
	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// 컨틀롤러 설정
	AIControllerClass = AFriendlyUnitController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 회전 설정
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);
	

	// 변수 초기화
	TargetOverlapActor = nullptr;
	OverlapRadius = 500.0f;
	AttackDelay = 1.5f;
	LastAttackTime = 0.0f;

	Tags.Empty();
	Tags.Add(TAG_PLAYERUNIT);
}

void AFriendlyAbleCharacterBase::InitializeUnitBase(
	const FFriendlyUnitDataTableRow* unitInfo,
	class AFriendlyUnitController* playerController,
	FName unitCode)
{
	PlayerController = playerController;
	UnitCode = unitCode;
	MaxHp = unitInfo->Hp;
	MaxMp = unitInfo->Mp;
	MinAttackPoint = unitInfo->MinAttackPoint;
	MaxAttackPoint = unitInfo->MaxAttackPoint;
	SellPrice = unitInfo->SellPrice;

	// 유닛의 최대 사거리는 감지범위를 넘을 수 없기 때문에 예외처리
	float attackRange = unitInfo->AttackRange;
	AttackRange = attackRange > OverlapRadius ? OverlapRadius : attackRange;
}

bool AFriendlyAbleCharacterBase::OnAttackRequested(FName weaponCode)
{
	if (!IsValid(AttackComponent)) return false;
	return AttackComponent->OnAttackRequested(weaponCode);
}

// 공격이 끝났을 때 공격객체에게 알리는 함수
void AFriendlyAbleCharacterBase::OnAttackFinished()
{
	// 필요한 경우 하위 클래스에서 구현
}

void AFriendlyAbleCharacterBase::BeginPlay()
{
	Super::BeginPlay();	
					 
	AGameLevelScriptActor* GameLevel = Cast<AGameLevelScriptActor>(GetWorld()->GetCurrentLevel()->GetLevelScriptActor());
	if (IsValid(GameLevel))
	{
		waveComponent = GameLevel->GetWaveComponent();
	}

	UAnimInstance* animInst = GetMesh()->GetAnimInstance();
	if (IsValid(animInst))
	{
		AnimInstance = Cast<UFriendlyAbleUnitAnimInstance>(animInst);
		AnimInstance->OnInitializeAinmInstance(this);

		// 공격 끝 콜백 등록
		AnimInstance->OnAttackFinished.AddUObject(
			this, &ThisClass::CALLBACK_OnAttackFinished);
	}
		
	// 공격 컴포넌트 초기화
	if (IsValid(AttackComponent))
	{
		AttackComponent->OnBeginPlayInitialize(this);
	}
}

void AFriendlyAbleCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 적 탐지 로직
	AutoSurchEnemy();
	
	// 다른 행동을 사용중이 아닐때 자동공격실행
	if(PlayerController)
		IsUseAction = PlayerController->GetIsUseBehaviorAction();
	
	if (!IsUseAction || IsSelectAttack) {
		// 캐릭터 회전 처리
		LockAtTarget(DeltaTime, 5.0f);

		// 자동공격
		AutoAttack();
	}
}

void AFriendlyAbleCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// 적 탐지 로직
void AFriendlyAbleCharacterBase::AutoSurchEnemy()
{
	// 감지된 적이 없다면 탐색로직을 계속 실행
	/*if (TargetOverlapActor == nullptr)
		CheckEnemyOverlap();
	else*/
	CheckEnemyOverlap();
	if(TargetOverlapActor != nullptr)
	{	// 타깃을 감지하여 가지고 있지만, 감지범위를 벗어났다면 로스트 처리
		float TargetDistance = FVector::Distance(GetActorLocation(), TargetOverlapActor->GetActorLocation());
		if(!IsSelectAttack)
		{
			if (TargetDistance > OverlapRadius)
				TargetOverlapActor = nullptr;
		}
	}
}

void AFriendlyAbleCharacterBase::AutoAttack()
{	// 예외처리
	if (TargetOverlapActor == nullptr) return;
	
	float TargetDistance = FVector::Distance(GetActorLocation(), TargetOverlapActor->GetActorLocation());
	
	// 타겟과의 거리가 공격 범위보다 먼 경우와 홀드 사용중이 아닐 경우
	if (TargetDistance > AttackRange && !PlayerController->GetIsUseHoldAction()) {
		PlayerController->SetBehaviorConditionCheck(false, false, true);
		PlayerController->SetTargetLocation(GetActorLocation(), TargetOverlapActor->GetActorLocation());
	}
	//타겟이 공격 범위 내에 있을 경우
	else
	{	// 유효성 다시 확인
		if (TargetOverlapActor == nullptr) return;

		// 자동 이동 멈춤
		PlayerController->UnitStopMovement();

		// 딜레이 체크
		float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime < LastAttackTime + AttackDelay) return;
		LastAttackTime = CurrentTime;

		// 공격 요청
		bool attackCheck = AttackComponent->OnAttackRequested(UseWeaponCode);

		// 공격이 실패했다면 타겟 초기화
		if (!attackCheck) TargetOverlapActor = nullptr;
		
	}
}

// 적을 바라보도록 하는 함수
void AFriendlyAbleCharacterBase::LockAtTarget(float dt, float roattionSpeed)
{
	if (!TargetOverlapActor)
	{
		FRotator ControllerRotation = GetControlRotation();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), ControllerRotation, dt, roattionSpeed));
	}
	else
	{
		// 적을 감지했을 때 목표물 방향으로 회전
		FVector TargetLocation = TargetOverlapActor->GetActorLocation();
		FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
		FRotator NewRotation = Direction.Rotation();

		// 회전 적용
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), NewRotation, dt, roattionSpeed));
	}

	// 컨트롤러 회전도 함께 업데이트	
	if (PlayerController)
	{
		PlayerController->SetControlRotation(GetActorRotation());
	}
}

void AFriendlyAbleCharacterBase::SelectUnit()
{
	SelectionDecal->SetVisibility(true);
}

void AFriendlyAbleCharacterBase::DeSelectUnit()
{	
	SelectionDecal->SetVisibility(false);
}

void AFriendlyAbleCharacterBase::UnitMoveCommand(FVector targetPoint)
{
	if (IsValid(GetController()))
	{
		// 재생중인 애니메이션이 있다면 정지
		if(AnimInstance->CurrentMontage != nullptr)
			AnimInstance->StopAnimMontage();

		PlayerController->SetBehaviorConditionCheck(false, false, true);
		PlayerController->SetTargetLocation(GetActorLocation(), targetPoint);
	}	
}

void AFriendlyAbleCharacterBase::CALLBACK_OnAttackFinished()
{
	// 공격이 끝났음을 공격 컴포넌트에게 알립니다.
	if (IsValid(AttackComponent))
		AttackComponent->OnAttackFinished();
}

// 범위 내에 가장 가까운 액터를 찾기
void AFriendlyAbleCharacterBase::CheckEnemyOverlap()
{
	FVector UnitLocation = GetActorLocation();

	TArray<AActor*> actorsToIgnore{ this };
	TArray<AActor*> OverlappingActors;

	UKismetSystemLibrary::SphereOverlapActors(
		this, UnitLocation, OverlapRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>>(), nullptr,
		actorsToIgnore, OverlappingActors);

	// 목표 액터와의 거리
	float distance = 10000.0f;

	if (OverlappingActors.Num() == 0) return;

	for (AActor* actor : OverlappingActors)
	{
		AEnemyCharacterBase* enemy = Cast<AEnemyCharacterBase>(actor);
		if(IsValid(enemy))
		{
			if (enemy->ActorHasTag(TAG_ENEMY))
			{
				float tempDistance = FVector::Distance(UnitLocation, enemy->GetActorLocation());

				// 기존의 액터들보다 거리가 가깝다면 갱신
				if (distance > tempDistance)
					TargetOverlapActor = enemy;
			}
		}
	}
}

void AFriendlyAbleCharacterBase::RollBackOverlapRange()
{
	IsSelectAttack = false;
}

void AFriendlyAbleCharacterBase::UnitAttack(AActor* targetActor)
{
	PlayerController->SetBehaviorConditionCheck(false, false, false);

	// 공격 객체 설정
	IsSelectAttack = true;
	TargetOverlapActor = targetActor;
	
	// 일시적으로 넓어진 탐지범위를 10초뒤에 다시 되돌리도록 타이머 설정
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::RollBackOverlapRange), 5.f, false);
}

void AFriendlyAbleCharacterBase::UnitHold()
{	
	PlayerController->SetBehaviorConditionCheck(true, false, false);
}

void AFriendlyAbleCharacterBase::UnitPetroll(FVector targetLocation)
{	
	// 비헤이비어 셀렉터에 사용될 변수 셋팅
	PlayerController->SetBehaviorConditionCheck(false, true, false);
	PlayerController->SetTargetLocation(GetActorLocation(), targetLocation);
}

void AFriendlyAbleCharacterBase::UnitSell()
{
	// 값이 설정 되지 않은 유닛은 판매 불가
	if (SellPrice == 0.0f) return;

	AGamePlayerController* GamePlayerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	GamePlayerController->MainContainerWidget->BP_TopContainerWidget->UpdateEnemyReward(SellPrice);

	// 무기 삭제
	AttackComponent->DeleteWeapon();

	// 판매완료 후, 유닛 삭제
	Destroy();
}


// 스킬 버튼 인터페이스 바인딩
// 실제 구현은 자식 클래스에서 구현
bool AFriendlyAbleCharacterBase::SkillBtn01()
{
	return false;
}
bool AFriendlyAbleCharacterBase::SkillBtn02()
{
	return false;
}
bool AFriendlyAbleCharacterBase::SkillBtn03()
{
	return false;
}
bool AFriendlyAbleCharacterBase::SkillBtn04()
{
	return false;
}
bool AFriendlyAbleCharacterBase::SkillBtn05()
{
	return false;
}
bool AFriendlyAbleCharacterBase::SkillBtn06()
{
	return false;
}
bool AFriendlyAbleCharacterBase::SkillBtn07()
{
	return false;
}
bool AFriendlyAbleCharacterBase::SkillBtn08()
{
	return false;
}