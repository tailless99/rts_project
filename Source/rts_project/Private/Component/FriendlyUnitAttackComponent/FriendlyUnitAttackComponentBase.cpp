#include "Component/FriendlyUnitAttackComponent/FriendlyUnitAttackComponentBase.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "Actor/EnemyCharacter/EnemyCharacterBase.h"
#include "AnimInstance/FriendlyAbleUnitAnimInstance.h"
#include "DataTableRow/WeaponInfoTableRow.h"
#include "Actor/Weapon/WeaponBase.h"
#include "rts_project/rts_project.h"
#include "Kismet/GameplayStatics.h"



UFriendlyUnitAttackComponentBase::UFriendlyUnitAttackComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_WEAPONINFO(
		TEXT("/Script/Engine.DataTable'/Game/DataTableRow/DT_WeaponDataRow.DT_WeaponDataRow'"));

	if (DT_WEAPONINFO.Succeeded())
	{
		WeaponInfoDataTable = DT_WEAPONINFO.Object;
	}
}

void UFriendlyUnitAttackComponentBase::BeginPlay()
{
	Super::BeginPlay();
}

void UFriendlyUnitAttackComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFriendlyUnitAttackComponentBase::OnBeginPlayInitialize(AFriendlyAbleCharacterBase* ownerPlayerUnit)
{
	PlayerUnit = ownerPlayerUnit;
	UFriendlyAbleUnitAnimInstance* AnimInstance = PlayerUnit->GetAnimInstance();

	// ���� ���� Ȱ��ȭ/��Ȱ��ȭ �ݹ� ���
	AnimInstance->OnAttackAreaCheckStarted.AddUObject(this, &ThisClass::CALLBACK_OnAttackAreaCheckStarted);
	AnimInstance->OnAttackAreaCheckFinished.AddUObject(this, &ThisClass::CALLBACK_OnAttackAreaCheckFinished);

}

// ���� ��û �Լ�
bool UFriendlyUnitAttackComponentBase::OnAttackRequested(FName weaponCode)
{
	// ���� ������ ����ϴ�.
	const FWeaponInfoTableRow* weaponInfo = GetWeaponInfo(weaponCode);

	// ���� ������ ��ȿ���� ���� ��� ���� ��û ����
	if (weaponInfo == nullptr) return false;

	// �ִϸ��̼� ��Ÿ�ְ� ��ȿ���� ���� ��� ���� ��û ����
	if (!IsValid(weaponInfo->UseAttackAnimMontage)) return false;

	// �ִϸ��̼� ���
	PlayerUnit->PlayAnimMontage(
		weaponInfo->UseAttackAnimMontage);

	// �������� ���Ⱑ �����ϴ� ���
	if (IsValid(EquippedWeapon))
	{
		// ���� ���۵�
		EquippedWeapon->SetTarget(PlayerUnit->TargetOverlapActor);
		EquippedWeapon->OnAttackStarted();
	}

	return true;
}

void UFriendlyUnitAttackComponentBase::OnAttackFinished()
{
	// �������� ���Ⱑ �����ϴ� ���
	if (IsValid(EquippedWeapon))
	{
		// ���� ���� ��Ȱ��ȭ
		EquippedWeapon->EnableAttackArea(false);

		// ���� �����
		EquippedWeapon->OnAttackFinished();
		
		
	}
}

// ���⸦ �����ϴ� �Լ�
AWeaponBase* UFriendlyUnitAttackComponentBase::EquipWeapon(
	FName weaponCode, 
	USceneComponent* attachTo, 
	FTransform transform, 
	FName socketName)
{
	// ���� ������ ����ϴ�.
	const FWeaponInfoTableRow* weaponInfo = GetWeaponInfo(weaponCode);

	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// ���� ���͸� �����մϴ�.
	AWeaponBase* weaponActor = GetWorld()->SpawnActor<AWeaponBase>(
		weaponInfo->WeaponClass,
		transform,
		spawnParam);

	// ���ⰴü�� �̿��Ͽ� ���� �� ���͸� �����Կ� ���� �ݹ� ���
	weaponActor->OnDetected.AddUObject(this, &ThisClass::CALLBACK_OnAttackDetected);

	// ���� ������ �����մϴ�.
	weaponActor->SetWeaponInfo(weaponInfo);

	// ������ ������Ʈ�� Attach ��ŵ�ϴ�.
	if (IsValid(attachTo))
	{
		weaponActor->AttachToComponent(
			attachTo,
			FAttachmentTransformRules::KeepRelativeTransform,
			socketName);
		weaponActor->SetActorRelativeLocation(FVector::ZeroVector);
		weaponActor->SetActorRelativeRotation(FRotator::ZeroRotator);
	}

	return EquippedWeapon = weaponActor;

}

void UFriendlyUnitAttackComponentBase::DeleteWeapon()
{
	EquippedWeapon->Destroy();
	EquippedWeapon = nullptr;
}

const FWeaponInfoTableRow* UFriendlyUnitAttackComponentBase::GetWeaponInfo(const FName& weaponCode) const
{
	FString contextString;
	return WeaponInfoDataTable->FindRow<FWeaponInfoTableRow>(weaponCode, contextString);
}

void UFriendlyUnitAttackComponentBase::CALLBACK_OnAttackAreaCheckStarted()
{
	// �������� ���Ⱑ �����ϴ� ���
	if (IsValid(EquippedWeapon))
	{
		// ���� ���� Ȱ��ȭ
		EquippedWeapon->EnableAttackArea(true);
	}
}

void UFriendlyUnitAttackComponentBase::CALLBACK_OnAttackAreaCheckFinished()
{
	// �������� ���Ⱑ �����ϴ� ���
	if (IsValid(EquippedWeapon))
	{
		// ���� ���� ��Ȱ��ȭ
		EquippedWeapon->EnableAttackArea(false);
		
		// ���� ��ü���� ������ �������� �˸�
		PlayerUnit->OnAttackFinished();
	}
}

void UFriendlyUnitAttackComponentBase::CALLBACK_OnAttackDetected(AActor* newDetected)
{
	// ����ó��
	if (!IsValid(PlayerUnit)) return;

	float damage = FMath::RandRange(PlayerUnit->MinAttackPoint, PlayerUnit->MaxAttackPoint);
	AEnemyCharacterBase* Enemy = Cast<AEnemyCharacterBase>(newDetected);
	
	// ������ ����
	UGameplayStatics::ApplyDamage(Enemy, damage, PlayerUnit->GetController(), PlayerUnit, UDamageType::StaticClass());
}

