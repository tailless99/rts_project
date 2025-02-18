#include "Actor/Weapon/WeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actor/EnemyCharacter/EnemyCharacterBase.h"


AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultWeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DEF_ROOT"));
	SetRootComponent(DefaultWeaponRoot);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_WEAPON"));
	WeaponMesh->SetupAttachment(GetRootComponent());

	// 소켓 이름 설정
	AttackAreaCheckStartSocketName = TEXT("Socket_Start");
	AttackAreaCheckEndSocketName = TEXT("Socket_End");
}

void AWeaponBase::SetWeaponInfo(const FWeaponInfoTableRow* weaponInfo)
{
	WeaponInfo = weaponInfo;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}


void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::OnAttackStarted()
{
	DetectedActors.Empty();
}

void AWeaponBase::OnAttackFinished()
{
	DetectedActors.Empty();
}

void AWeaponBase::EnableAttackArea(bool bEnable)
{
	bAttackAreaEnable = bEnable;
}

void AWeaponBase::SetTarget(AActor* Enemy)
{
	if(Enemy)
		TargetEnemy = Cast<AEnemyCharacterBase>(Enemy);
}



