#include "Actor/UseableUnit/FriendlyCharacter/UnComon/UnComonCharacter_Lupi.h"
#include "Actor/LocationActor/PlayerUnitTypeASpawnLocation.h"
#include "Actor/LocationActor/UnitSpawnLocation.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/Level/WaveControllerComponent.h"
#include "Component/FriendlyUnitAttackComponent/FriendlyUnitAttackComponentBase.h"
#include "rts_project/rts_project.h"
#include "EngineUtils.h"



AUnComonCharacter_Lupi::AUnComonCharacter_Lupi()
{
	// 회전 설정
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);

	// 공격 컴포넌트 생성
	AttackComponent = CreateDefaultSubobject<UFriendlyUnitAttackComponentBase>(TEXT("ATTACK_COMP"));

	// 유닛 코드 설정
	UnitCode = TEXT("0001");

	// 사용 무기 코드 설정
	UseWeaponCode = TEXT("0001");
}

void AUnComonCharacter_Lupi::BeginPlay()
{
	Super::BeginPlay();

	// 이동 위치 설정
	SetMovePointLocation();

	// 무기 장착
	FTransform weaponSocketTransform = GetMesh()->GetSocketTransform(TEXT("Socket_Weapon_Sword"));
	AttackComponent->EquipWeapon(UseWeaponCode, GetMesh(), weaponSocketTransform, TEXT("Socket_Weapon_Sword"));
}

void AUnComonCharacter_Lupi::SetMovePointLocation()
{
	for (TActorIterator<APlayerUnitTypeASpawnLocation> It(GetWorld()); It; ++It)
	{
		APlayerUnitTypeASpawnLocation* Actor = *It;

		if (FName(Actor->GetActorNameOrLabel()) == TEXT("TypeA_PointA")) {
			StorageStartLocation = Actor->GetActorLocation();
		}
		if (FName(Actor->GetActorNameOrLabel()) == TEXT("TypeA_PointB")) {
			StorageEndLocation = Actor->GetActorLocation();
		}
	}

	for (TActorIterator<AActorSpawnLocationBase> It(GetWorld()); It; ++It) {
		AActorSpawnLocationBase* Actor = *It;

		if (AUnitSpawnLocation* unitSpawnLocation = Cast<AUnitSpawnLocation>(Actor)) {
			UnitSpawnLocation = unitSpawnLocation->GetActorLocation();
		}
	}
}

bool AUnComonCharacter_Lupi::SkillBtn01()
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation;
	NewLocation.Z = CurrentLocation.Z; // Z 위치는 현재 위치 유지

	bool bFoundEmptySpace = false; // 결과확인
	
	// 창고로 이동하는 경우
	if (!IsMoveStorage)
	{
		// 빈 공간 찾기
		for (int32 i = 0; i < 100; ++i) // 100번 샘플링 시도
		{
			NewLocation.X = FMath::RandRange(StorageStartLocation.X, StorageEndLocation.X);
			NewLocation.Y = FMath::RandRange(StorageStartLocation.Y, StorageEndLocation.Y);

			FHitResult HitResult;
			// 새 위치에 다른 액터가 겹치는지 체크
			if (!GetWorld()->SweepSingleByProfile(HitResult, CurrentLocation, NewLocation, FQuat::Identity, TEXT("OverlapAllDynamic"), FCollisionShape::MakeSphere(10.f)))
			{
				bFoundEmptySpace = true;
				break; // 빈 공간 찾으면 반복 종료
			}
		}

		// 빈 공간이 발견된 경우 이동
		if (bFoundEmptySpace)
		{
			this->SetActorLocation(NewLocation);
		}
	}
	// 라인으로 복귀하는 경우
	else
	{ 
		NewLocation = UnitSpawnLocation;
		

		FHitResult HitResult;
		// 새 위치에 다른 액터가 겹치는지 체크
		if (!GetWorld()->SweepSingleByProfile(HitResult, CurrentLocation, NewLocation, FQuat::Identity, TEXT("OverlapAllDynamic"), FCollisionShape::MakeSphere(10.f)))
		{
			bFoundEmptySpace = true;
		}
		else {
			// 빈 공간 찾기
			for (int32 i = 0; i < 100; ++i) // 100번 샘플링 시도
			{
				NewLocation.X = FMath::RandRange(NewLocation.X, NewLocation.X + i);
				NewLocation.Y = FMath::RandRange(NewLocation.Y, NewLocation.Y + i);

				// 새 위치에 다른 액터가 겹치는지 체크
				if (!GetWorld()->SweepSingleByProfile(HitResult, CurrentLocation, NewLocation, FQuat::Identity, TEXT("OverlapAllDynamic"), FCollisionShape::MakeSphere(10.f)))
				{
					bFoundEmptySpace = true;
					break; // 빈 공간 찾으면 반복 종료
				}
			}
		}

		// 빈 공간이 발견된 경우 이동
		if (bFoundEmptySpace)
		{
			SetActorLocation(NewLocation);
		}
	}

	IsMoveStorage = !IsMoveStorage;
	return bFoundEmptySpace ? true : false;
}

bool AUnComonCharacter_Lupi::SkillBtn05()
{
	// 초기화
	MixtureUnits.Empty();

	for (TActorIterator<AUnComonCharacter_Lupi> It(GetWorld()); It; ++It)
	{
		AUnComonCharacter_Lupi* Actor = *It;

		if (Actor && MixtureUnits.Num() < 3) {
			MixtureUnits.Add(Actor);
		}
	}

	if (MixtureUnits.Num() == 3) {
		waveComponent->CallbackRequest_SpawnUnit(FName(TEXT("0004")));

		// 재료 액터들 삭제
		for (AActor* actor : MixtureUnits)
		{
			AUnComonCharacter_Lupi* playerUnit = Cast<AUnComonCharacter_Lupi>(actor);
			playerUnit->AttackComponent->DeleteWeapon();
			playerUnit->Destroy();
		}
		return true;
	}

	return false;
}