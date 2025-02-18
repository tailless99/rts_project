#include "Actor/UseableUnit/FriendlyCharacter/Common/CommonCharacter_ShotgunMan.h"
#include "Actor/EnemyCharacter/EnemyCharacterBase.h"
#include "Component/FriendlyUnitAttackComponent/FriendlyUnitAttackComponentBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "rts_project/rts_project.h"



ACommonCharacter_ShotgunMan::ACommonCharacter_ShotgunMan()
{
	// 회전 설정
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);

	// 공격 컴포넌트 생성
	AttackComponent = CreateDefaultSubobject<UFriendlyUnitAttackComponentBase>(TEXT("ATTACK_COMP"));

	// 유닛 코드 설정
	UnitCode = TEXT("0005");

	// 사용 무기 코드 설정
	UseWeaponCode = TEXT("0004");

	// 딜레이 설정
	AttackDelay = 2.0f;

	// 1번 스킬을 사용을 확인하기 위한 부울 변수
	IsUseSpeedDownSkill = true;

	// 1번 스킬에 사용되는 적 탐지 범위
	FindEnemyAreaRadius = 300.f;

	// 이동 감소 수치
	DebuffSpeedDown = 500.0f;
}

void ACommonCharacter_ShotgunMan::BeginPlay()
{
	Super::BeginPlay();

	// 무기 장착
	FTransform weaponSocketTransform = GetMesh()->GetSocketTransform(TEXT("Socket_Weapon_Gun"));
	AttackComponent->EquipWeapon(UseWeaponCode, GetMesh(), weaponSocketTransform, TEXT("Socket_Weapon_Gun"));
}

void ACommonCharacter_ShotgunMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1번 스킬을 사용할 때
	if (IsUseSpeedDownSkill)
	{
		// 적이 범위 내로 들어오는지 체크하는 함수
		CheckEnemyDetectedArea();
	}
}

// 적이 범위 내로 들어오는지 체크하는 함수
void ACommonCharacter_ShotgunMan::CheckEnemyDetectedArea()
{
	FVector UnitLocation = GetActorLocation();

	TArray<AActor*> actorsToIgnore{ this };
	TArray<AActor*> OverlappingActors;

	UKismetSystemLibrary::SphereOverlapActors(
		this, UnitLocation, FindEnemyAreaRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>>(), nullptr,
		actorsToIgnore, OverlappingActors);

	if (OverlappingActors.Num() == 0) return;

	// 범위내로 새로 들어온 경우 추가
	for (AActor* actor : OverlappingActors)
	{
		AEnemyCharacterBase* enemy = Cast<AEnemyCharacterBase>(actor);
		if (IsValid(enemy) && enemy->ActorHasTag(TAG_ENEMY))
		{
			// 감지된 적이 있을 때
			DetectedEnemys.Add(enemy);

			// 에너미 이동속도 변경 요청
			enemy->CALLBACK_EnemySpeedDownEventStarted(DebuffSpeedDown);
		}
	}

	// 범위 내에 있던 적이 범위 밖으로 나간 경우 체크
	if (DetectedEnemys.Num() > 0)
	{
		for (AEnemyCharacterBase* enemy : DetectedEnemys)
		{
			if (!OverlappingActors.Contains(enemy)) {
				if (!enemy) break;

				DetectedEnemys.Remove(enemy);
				// 속도를 원래대로 돌려줌
				enemy->CALLBACK_EnemySpeedDownEventEnded();
			}
		}
	}
}

// 적 이동속도 감소시키는 패시브 스킬
bool ACommonCharacter_ShotgunMan::SkillBtn01()
{
	IsUseSpeedDownSkill = true;
	return false;
}

bool ACommonCharacter_ShotgunMan::SkillBtn05()
{
	return false;
}
