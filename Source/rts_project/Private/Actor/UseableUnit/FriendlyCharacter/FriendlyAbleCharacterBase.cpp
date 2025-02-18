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
	
	// ĸ�� �ݶ��̴� �⺻ ����
	CapsuleCollider = GetCapsuleComponent();
	CapsuleCollider->SetCapsuleRadius(30.0f);
	CapsuleCollider->SetCapsuleHalfHeight(100.0f);
	//RootComponent = CapsuleCollider;
	SetRootComponent(CapsuleCollider);

	// ��Į ������Ʈ ����
	SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionDecal"));
	SelectionDecal->SetupAttachment(GetRootComponent());
	SelectionDecal->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
	SelectionDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	SelectionDecal->DecalSize = FVector(15.0, 30.0, 30.0);
	SelectionDecal->SetVisibility(false);


	if (MT_SELECTED_EFFECT.Succeeded()) {
		SelectionDecal->SetDecalMaterial(MT_SELECTED_EFFECT.Object);
	}

	// �޽� ����
	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// ��Ʋ�ѷ� ����
	AIControllerClass = AFriendlyUnitController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// ȸ�� ����
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);
	

	// ���� �ʱ�ȭ
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

	// ������ �ִ� ��Ÿ��� ���������� ���� �� ���� ������ ����ó��
	float attackRange = unitInfo->AttackRange;
	AttackRange = attackRange > OverlapRadius ? OverlapRadius : attackRange;
}

bool AFriendlyAbleCharacterBase::OnAttackRequested(FName weaponCode)
{
	if (!IsValid(AttackComponent)) return false;
	return AttackComponent->OnAttackRequested(weaponCode);
}

// ������ ������ �� ���ݰ�ü���� �˸��� �Լ�
void AFriendlyAbleCharacterBase::OnAttackFinished()
{
	// �ʿ��� ��� ���� Ŭ�������� ����
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

		// ���� �� �ݹ� ���
		AnimInstance->OnAttackFinished.AddUObject(
			this, &ThisClass::CALLBACK_OnAttackFinished);
	}
		
	// ���� ������Ʈ �ʱ�ȭ
	if (IsValid(AttackComponent))
	{
		AttackComponent->OnBeginPlayInitialize(this);
	}
}

void AFriendlyAbleCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �� Ž�� ����
	AutoSurchEnemy();
	
	// �ٸ� �ൿ�� ������� �ƴҶ� �ڵ����ݽ���
	if(PlayerController)
		IsUseAction = PlayerController->GetIsUseBehaviorAction();
	
	if (!IsUseAction || IsSelectAttack) {
		// ĳ���� ȸ�� ó��
		LockAtTarget(DeltaTime, 5.0f);

		// �ڵ�����
		AutoAttack();
	}
}

void AFriendlyAbleCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// �� Ž�� ����
void AFriendlyAbleCharacterBase::AutoSurchEnemy()
{
	// ������ ���� ���ٸ� Ž�������� ��� ����
	/*if (TargetOverlapActor == nullptr)
		CheckEnemyOverlap();
	else*/
	CheckEnemyOverlap();
	if(TargetOverlapActor != nullptr)
	{	// Ÿ���� �����Ͽ� ������ ������, ���������� ����ٸ� �ν�Ʈ ó��
		float TargetDistance = FVector::Distance(GetActorLocation(), TargetOverlapActor->GetActorLocation());
		if(!IsSelectAttack)
		{
			if (TargetDistance > OverlapRadius)
				TargetOverlapActor = nullptr;
		}
	}
}

void AFriendlyAbleCharacterBase::AutoAttack()
{	// ����ó��
	if (TargetOverlapActor == nullptr) return;
	
	float TargetDistance = FVector::Distance(GetActorLocation(), TargetOverlapActor->GetActorLocation());
	
	// Ÿ�ٰ��� �Ÿ��� ���� �������� �� ���� Ȧ�� ������� �ƴ� ���
	if (TargetDistance > AttackRange && !PlayerController->GetIsUseHoldAction()) {
		PlayerController->SetBehaviorConditionCheck(false, false, true);
		PlayerController->SetTargetLocation(GetActorLocation(), TargetOverlapActor->GetActorLocation());
	}
	//Ÿ���� ���� ���� ���� ���� ���
	else
	{	// ��ȿ�� �ٽ� Ȯ��
		if (TargetOverlapActor == nullptr) return;

		// �ڵ� �̵� ����
		PlayerController->UnitStopMovement();

		// ������ üũ
		float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime < LastAttackTime + AttackDelay) return;
		LastAttackTime = CurrentTime;

		// ���� ��û
		bool attackCheck = AttackComponent->OnAttackRequested(UseWeaponCode);

		// ������ �����ߴٸ� Ÿ�� �ʱ�ȭ
		if (!attackCheck) TargetOverlapActor = nullptr;
		
	}
}

// ���� �ٶ󺸵��� �ϴ� �Լ�
void AFriendlyAbleCharacterBase::LockAtTarget(float dt, float roattionSpeed)
{
	if (!TargetOverlapActor)
	{
		FRotator ControllerRotation = GetControlRotation();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), ControllerRotation, dt, roattionSpeed));
	}
	else
	{
		// ���� �������� �� ��ǥ�� �������� ȸ��
		FVector TargetLocation = TargetOverlapActor->GetActorLocation();
		FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
		FRotator NewRotation = Direction.Rotation();

		// ȸ�� ����
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), NewRotation, dt, roattionSpeed));
	}

	// ��Ʈ�ѷ� ȸ���� �Բ� ������Ʈ	
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
		// ������� �ִϸ��̼��� �ִٸ� ����
		if(AnimInstance->CurrentMontage != nullptr)
			AnimInstance->StopAnimMontage();

		PlayerController->SetBehaviorConditionCheck(false, false, true);
		PlayerController->SetTargetLocation(GetActorLocation(), targetPoint);
	}	
}

void AFriendlyAbleCharacterBase::CALLBACK_OnAttackFinished()
{
	// ������ �������� ���� ������Ʈ���� �˸��ϴ�.
	if (IsValid(AttackComponent))
		AttackComponent->OnAttackFinished();
}

// ���� ���� ���� ����� ���͸� ã��
void AFriendlyAbleCharacterBase::CheckEnemyOverlap()
{
	FVector UnitLocation = GetActorLocation();

	TArray<AActor*> actorsToIgnore{ this };
	TArray<AActor*> OverlappingActors;

	UKismetSystemLibrary::SphereOverlapActors(
		this, UnitLocation, OverlapRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>>(), nullptr,
		actorsToIgnore, OverlappingActors);

	// ��ǥ ���Ϳ��� �Ÿ�
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

				// ������ ���͵麸�� �Ÿ��� �����ٸ� ����
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

	// ���� ��ü ����
	IsSelectAttack = true;
	TargetOverlapActor = targetActor;
	
	// �Ͻ������� �о��� Ž�������� 10�ʵڿ� �ٽ� �ǵ������� Ÿ�̸� ����
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
	// �����̺�� �����Ϳ� ���� ���� ����
	PlayerController->SetBehaviorConditionCheck(false, true, false);
	PlayerController->SetTargetLocation(GetActorLocation(), targetLocation);
}

void AFriendlyAbleCharacterBase::UnitSell()
{
	// ���� ���� ���� ���� ������ �Ǹ� �Ұ�
	if (SellPrice == 0.0f) return;

	AGamePlayerController* GamePlayerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	GamePlayerController->MainContainerWidget->BP_TopContainerWidget->UpdateEnemyReward(SellPrice);

	// ���� ����
	AttackComponent->DeleteWeapon();

	// �ǸſϷ� ��, ���� ����
	Destroy();
}


// ��ų ��ư �������̽� ���ε�
// ���� ������ �ڽ� Ŭ�������� ����
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