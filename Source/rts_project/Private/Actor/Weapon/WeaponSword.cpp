#include "Actor/Weapon/WeaponSword.h"
#include "rts_project/rts_project.h"
#include "Actor/EnemyCharacter/EnemyCharacterBase.h"



AWeaponSword::AWeaponSword()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SWORD(
		TEXT("/Script/Engine.StaticMesh'/Game/Resources/SwordShieldAnims/Weapon/SM_Sword_WSS.SM_Sword_WSS'"));

	if (SM_SWORD.Succeeded())
	{
		WeaponMesh->SetStaticMesh(SM_SWORD.Object);
	}

	WeaponMesh->SetCollisionProfileName(COLLISIONNAME_NOCOLLISION);
}

void AWeaponSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckAttackArea();
}

void AWeaponSword::CheckAttackArea()
{
	// 예외처리
	if (!bAttackAreaEnable) return;

	// 공격 영역 검사를 위해 두 끝점 위치를 계산함
	FVector startLocation, endLocation;
	UpdateAttackAreaCheckPoints(startLocation, endLocation);

	TArray<AActor*> actorToIgnore;
	TArray<FHitResult> hitResults;

	UKismetSystemLibrary::SphereTraceMultiByProfile(
		this, startLocation, endLocation, 10.0f,
		TEXT("OverlapAllDynamic"), false,
		actorToIgnore, EDrawDebugTrace::Type::None,
		hitResults, true);

	// 액터를 감지했을 경우
	if (hitResults.Num() > 0)
	{
		for (FHitResult hitResult : hitResults)
		{
			// 감지한 액터를 얻는다
			AEnemyCharacterBase* Enemy = Cast<AEnemyCharacterBase>(hitResult.GetActor());

			// 감지된 액터가 에너미이고, 이미 등록된 객체가 아닌 경우
			if (Enemy && !DetectedActors.Contains(Enemy))
			{
				DetectedActors.Add(Enemy);
				OnDetected.Broadcast(Enemy);
			}
		}
	}
}

void AWeaponSword::UpdateAttackAreaCheckPoints(FVector& out_start, FVector& out_end)
{
	if (!IsValid(WeaponMesh->GetStaticMesh())) return;

	out_start = WeaponMesh->GetSocketTransform(AttackAreaCheckStartSocketName).GetLocation();
	out_end = WeaponMesh->GetSocketTransform(AttackAreaCheckEndSocketName).GetLocation();

}