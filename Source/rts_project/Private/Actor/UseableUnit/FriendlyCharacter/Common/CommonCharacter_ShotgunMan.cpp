#include "Actor/UseableUnit/FriendlyCharacter/Common/CommonCharacter_ShotgunMan.h"
#include "Actor/EnemyCharacter/EnemyCharacterBase.h"
#include "Component/FriendlyUnitAttackComponent/FriendlyUnitAttackComponentBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "rts_project/rts_project.h"



ACommonCharacter_ShotgunMan::ACommonCharacter_ShotgunMan()
{
	// ȸ�� ����
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);

	// ���� ������Ʈ ����
	AttackComponent = CreateDefaultSubobject<UFriendlyUnitAttackComponentBase>(TEXT("ATTACK_COMP"));

	// ���� �ڵ� ����
	UnitCode = TEXT("0005");

	// ��� ���� �ڵ� ����
	UseWeaponCode = TEXT("0004");

	// ������ ����
	AttackDelay = 2.0f;

	// 1�� ��ų�� ����� Ȯ���ϱ� ���� �ο� ����
	IsUseSpeedDownSkill = true;

	// 1�� ��ų�� ���Ǵ� �� Ž�� ����
	FindEnemyAreaRadius = 300.f;

	// �̵� ���� ��ġ
	DebuffSpeedDown = 500.0f;
}

void ACommonCharacter_ShotgunMan::BeginPlay()
{
	Super::BeginPlay();

	// ���� ����
	FTransform weaponSocketTransform = GetMesh()->GetSocketTransform(TEXT("Socket_Weapon_Gun"));
	AttackComponent->EquipWeapon(UseWeaponCode, GetMesh(), weaponSocketTransform, TEXT("Socket_Weapon_Gun"));
}

void ACommonCharacter_ShotgunMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1�� ��ų�� ����� ��
	if (IsUseSpeedDownSkill)
	{
		// ���� ���� ���� �������� üũ�ϴ� �Լ�
		CheckEnemyDetectedArea();
	}
}

// ���� ���� ���� �������� üũ�ϴ� �Լ�
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

	// �������� ���� ���� ��� �߰�
	for (AActor* actor : OverlappingActors)
	{
		AEnemyCharacterBase* enemy = Cast<AEnemyCharacterBase>(actor);
		if (IsValid(enemy) && enemy->ActorHasTag(TAG_ENEMY))
		{
			// ������ ���� ���� ��
			DetectedEnemys.Add(enemy);

			// ���ʹ� �̵��ӵ� ���� ��û
			enemy->CALLBACK_EnemySpeedDownEventStarted(DebuffSpeedDown);
		}
	}

	// ���� ���� �ִ� ���� ���� ������ ���� ��� üũ
	if (DetectedEnemys.Num() > 0)
	{
		for (AEnemyCharacterBase* enemy : DetectedEnemys)
		{
			if (!OverlappingActors.Contains(enemy)) {
				if (!enemy) break;

				DetectedEnemys.Remove(enemy);
				// �ӵ��� ������� ������
				enemy->CALLBACK_EnemySpeedDownEventEnded();
			}
		}
	}
}

// �� �̵��ӵ� ���ҽ�Ű�� �нú� ��ų
bool ACommonCharacter_ShotgunMan::SkillBtn01()
{
	IsUseSpeedDownSkill = true;
	return false;
}

bool ACommonCharacter_ShotgunMan::SkillBtn05()
{
	return false;
}
