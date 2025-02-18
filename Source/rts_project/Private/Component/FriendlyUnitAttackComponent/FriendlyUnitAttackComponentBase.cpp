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

	// 공격 영역 활성화/비활성화 콜백 등록
	AnimInstance->OnAttackAreaCheckStarted.AddUObject(this, &ThisClass::CALLBACK_OnAttackAreaCheckStarted);
	AnimInstance->OnAttackAreaCheckFinished.AddUObject(this, &ThisClass::CALLBACK_OnAttackAreaCheckFinished);

}

// 공격 요청 함수
bool UFriendlyUnitAttackComponentBase::OnAttackRequested(FName weaponCode)
{
	// 무기 정보를 얻습니다.
	const FWeaponInfoTableRow* weaponInfo = GetWeaponInfo(weaponCode);

	// 무기 정보가 유효하지 않은 경우 공격 요청 실패
	if (weaponInfo == nullptr) return false;

	// 애니메이션 몽타주가 유효하지 않은 경우 공격 요청 실패
	if (!IsValid(weaponInfo->UseAttackAnimMontage)) return false;

	// 애니메이션 재생
	PlayerUnit->PlayAnimMontage(
		weaponInfo->UseAttackAnimMontage);

	// 장착중인 무기가 존재하는 경우
	if (IsValid(EquippedWeapon))
	{
		// 공격 시작됨
		EquippedWeapon->SetTarget(PlayerUnit->TargetOverlapActor);
		EquippedWeapon->OnAttackStarted();
	}

	return true;
}

void UFriendlyUnitAttackComponentBase::OnAttackFinished()
{
	// 장착중인 무기가 존재하는 경우
	if (IsValid(EquippedWeapon))
	{
		// 공격 영역 비활성화
		EquippedWeapon->EnableAttackArea(false);

		// 공격 종료됨
		EquippedWeapon->OnAttackFinished();
		
		
	}
}

// 무기를 장착하는 함수
AWeaponBase* UFriendlyUnitAttackComponentBase::EquipWeapon(
	FName weaponCode, 
	USceneComponent* attachTo, 
	FTransform transform, 
	FName socketName)
{
	// 무기 정보를 얻습니다.
	const FWeaponInfoTableRow* weaponInfo = GetWeaponInfo(weaponCode);

	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 무기 액터를 생성합니다.
	AWeaponBase* weaponActor = GetWorld()->SpawnActor<AWeaponBase>(
		weaponInfo->WeaponClass,
		transform,
		spawnParam);

	// 무기객체를 이용하여 공격 중 액터를 감지함에 대한 콜백 등록
	weaponActor->OnDetected.AddUObject(this, &ThisClass::CALLBACK_OnAttackDetected);

	// 무기 정보를 설정합니다.
	weaponActor->SetWeaponInfo(weaponInfo);

	// 지정한 컴포넌트에 Attach 시킵니다.
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
	// 장착중인 무기가 존재하는 경우
	if (IsValid(EquippedWeapon))
	{
		// 공격 영역 활성화
		EquippedWeapon->EnableAttackArea(true);
	}
}

void UFriendlyUnitAttackComponentBase::CALLBACK_OnAttackAreaCheckFinished()
{
	// 장착중인 무기가 존재하는 경우
	if (IsValid(EquippedWeapon))
	{
		// 공격 영역 비활성화
		EquippedWeapon->EnableAttackArea(false);
		
		// 공격 객체에게 공격이 끝났음을 알림
		PlayerUnit->OnAttackFinished();
	}
}

void UFriendlyUnitAttackComponentBase::CALLBACK_OnAttackDetected(AActor* newDetected)
{
	// 예외처리
	if (!IsValid(PlayerUnit)) return;

	float damage = FMath::RandRange(PlayerUnit->MinAttackPoint, PlayerUnit->MaxAttackPoint);
	AEnemyCharacterBase* Enemy = Cast<AEnemyCharacterBase>(newDetected);
	
	// 데미지 적용
	UGameplayStatics::ApplyDamage(Enemy, damage, PlayerUnit->GetController(), PlayerUnit, UDamageType::StaticClass());
}

