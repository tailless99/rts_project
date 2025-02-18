#include "Actor/Weapon/WeaponPistolGun.h"
#include "rts_project/rts_project.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/EnemyCharacter/EnemyCharacterBase.h"



AWeaponPistolGun::AWeaponPistolGun()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SWORD(
		TEXT("/Script/Engine.StaticMesh'/Game/Resources/Lyzai/SM_Pistol.SM_Pistol'"));

	if (SM_SWORD.Succeeded())
	{
		WeaponMesh->SetStaticMesh(SM_SWORD.Object);
	}

	WeaponMesh->SetCollisionProfileName(COLLISIONNAME_NOCOLLISION);
}

void AWeaponPistolGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckAttackArea();
}

void AWeaponPistolGun::CheckAttackArea()
{
	// ����ó��
	if (!bAttackAreaEnable) return;
	
	// ���� ���� �˻縦 ���� �� ���� ��ġ�� �����
	FVector startLocation = GetActorLocation();
	FVector endLocation = TargetEnemy->GetActorLocation();

	TArray<AActor*> actorToIgnore;
	TArray<FHitResult> hitResults;

	UKismetSystemLibrary::SphereTraceMultiByProfile(
		this, startLocation, endLocation, 10.0f,
		TEXT("OverlapAllDynamic"), false,
		actorToIgnore, EDrawDebugTrace::Type::None,
		hitResults, true);

	// ���͸� �������� ���
	if (hitResults.Num() > 0)
	{
		for (FHitResult hitResult : hitResults)
		{
			// ������ ���͸� ��´�
			AEnemyCharacterBase* Enemy = Cast<AEnemyCharacterBase>(hitResult.GetActor());

			// ������ ���Ͱ� ���ʹ��̰�, �̹� ��ϵ� ��ü�� �ƴ� ���
			if (Enemy && !DetectedActors.Contains(Enemy))
			{
				DetectedActors.Add(Enemy);
				OnDetected.Broadcast(Enemy);
			}
		}
	}

	// �����ɽ�Ʈ �߻縦 ���ƴٸ� ���� ������ ��Ȱ��ȭ
	bAttackAreaEnable = false;
}
