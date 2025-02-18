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
	// ȸ�� ����
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);

	// ���� ������Ʈ ����
	AttackComponent = CreateDefaultSubobject<UFriendlyUnitAttackComponentBase>(TEXT("ATTACK_COMP"));

	// ���� �ڵ� ����
	UnitCode = TEXT("0001");

	// ��� ���� �ڵ� ����
	UseWeaponCode = TEXT("0001");
}

void AUnComonCharacter_Lupi::BeginPlay()
{
	Super::BeginPlay();

	// �̵� ��ġ ����
	SetMovePointLocation();

	// ���� ����
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
	NewLocation.Z = CurrentLocation.Z; // Z ��ġ�� ���� ��ġ ����

	bool bFoundEmptySpace = false; // ���Ȯ��
	
	// â��� �̵��ϴ� ���
	if (!IsMoveStorage)
	{
		// �� ���� ã��
		for (int32 i = 0; i < 100; ++i) // 100�� ���ø� �õ�
		{
			NewLocation.X = FMath::RandRange(StorageStartLocation.X, StorageEndLocation.X);
			NewLocation.Y = FMath::RandRange(StorageStartLocation.Y, StorageEndLocation.Y);

			FHitResult HitResult;
			// �� ��ġ�� �ٸ� ���Ͱ� ��ġ���� üũ
			if (!GetWorld()->SweepSingleByProfile(HitResult, CurrentLocation, NewLocation, FQuat::Identity, TEXT("OverlapAllDynamic"), FCollisionShape::MakeSphere(10.f)))
			{
				bFoundEmptySpace = true;
				break; // �� ���� ã���� �ݺ� ����
			}
		}

		// �� ������ �߰ߵ� ��� �̵�
		if (bFoundEmptySpace)
		{
			this->SetActorLocation(NewLocation);
		}
	}
	// �������� �����ϴ� ���
	else
	{ 
		NewLocation = UnitSpawnLocation;
		

		FHitResult HitResult;
		// �� ��ġ�� �ٸ� ���Ͱ� ��ġ���� üũ
		if (!GetWorld()->SweepSingleByProfile(HitResult, CurrentLocation, NewLocation, FQuat::Identity, TEXT("OverlapAllDynamic"), FCollisionShape::MakeSphere(10.f)))
		{
			bFoundEmptySpace = true;
		}
		else {
			// �� ���� ã��
			for (int32 i = 0; i < 100; ++i) // 100�� ���ø� �õ�
			{
				NewLocation.X = FMath::RandRange(NewLocation.X, NewLocation.X + i);
				NewLocation.Y = FMath::RandRange(NewLocation.Y, NewLocation.Y + i);

				// �� ��ġ�� �ٸ� ���Ͱ� ��ġ���� üũ
				if (!GetWorld()->SweepSingleByProfile(HitResult, CurrentLocation, NewLocation, FQuat::Identity, TEXT("OverlapAllDynamic"), FCollisionShape::MakeSphere(10.f)))
				{
					bFoundEmptySpace = true;
					break; // �� ���� ã���� �ݺ� ����
				}
			}
		}

		// �� ������ �߰ߵ� ��� �̵�
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
	// �ʱ�ȭ
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

		// ��� ���͵� ����
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