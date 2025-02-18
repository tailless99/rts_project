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
	// 예외처리
	if (!bAttackAreaEnable) return;
	
	// 공격 영역 검사를 위해 두 끝점 위치를 계산함
	FVector startLocation = GetActorLocation();
	FVector endLocation = TargetEnemy->GetActorLocation();

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

	// 레이케스트 발사를 마쳤다면 감지 영역을 비활성화
	bAttackAreaEnable = false;
}
