#include "Actor/UseableUnit/FriendlyCharacter/Common/CommonCharacter_DarkSwordMan.h"
#include "Actor/EnemyCharacter/EnemyCharacterBase.h"
#include "Component/FriendlyUnitAttackComponent/FriendlyUnitAttackComponentBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "rts_project/rts_project.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"



ACommonCharacter_DarkSwordMan::ACommonCharacter_DarkSwordMan()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_EXPLOSION(
		TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_B.P_Explosion_Big_B'"));

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PARTICLESYSTEM_COMP"));
	ParticleComponent->SetupAttachment(GetRootComponent());
	ParticleComponent->bAutoActivate = false;

	if (PS_EXPLOSION.Succeeded())
	{
		ParticleComponent->SetTemplate(PS_EXPLOSION.Object);
	}


	// 회전 설정
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);

	// 공격 컴포넌트 생성
	AttackComponent = CreateDefaultSubobject<UFriendlyUnitAttackComponentBase>(TEXT("ATTACK_COMP"));
	
	// 유닛 코드 설정
	UnitCode = TEXT("0004");

	// 사용 무기 코드 설정
	UseWeaponCode = TEXT("0003");	

	// 스턴 범위
	StunRadius = 300.f;

	// 스턴 시간
	StunTime = 1.f;

	// 파티클 시스템 비활성화
	IsActivatePaticle = false;
}

void ACommonCharacter_DarkSwordMan::BeginPlay()
{
	Super::BeginPlay();

	// 무기 장착
	FTransform weaponSocketTransform = GetMesh()->GetSocketTransform(TEXT("Socket_Weapon_DarkSword"));
	AttackComponent->EquipWeapon(UseWeaponCode, GetMesh(), weaponSocketTransform, TEXT("Socket_Weapon_DarkSword"));

	// 파티클 시스템이 끝나면 자동으로 정지
	ParticleComponent->OnSystemFinished.AddDynamic(this, &ThisClass::ActivateStunEffect);
}

void ACommonCharacter_DarkSwordMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACommonCharacter_DarkSwordMan::OnAttackFinished()
{
	Super::OnAttackFinished();

	int32 random = FMath::RandRange(1, 100);

	// 평타 공격이 끝났을 때, 확률에 의해 스킬 적용
	// 발동 확률 90퍼센트
	if (random < 101)
	{
		SkillBtn01();
	}
}

// 범위 스턴 로직
void ACommonCharacter_DarkSwordMan::StunEnemyLogic()
{
	FVector UnitLocation = GetActorLocation();

	TArray<AActor*> actorsToIgnore{ this };
	TArray<AActor*> OverlappingActors;

	UKismetSystemLibrary::SphereOverlapActors(
		this, UnitLocation, StunRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>>(), nullptr,
		actorsToIgnore, OverlappingActors);

	// 범위내로 새로 들어온 경우 추가
	for (AActor* actor : OverlappingActors)
	{
		AEnemyCharacterBase* enemy = Cast<AEnemyCharacterBase>(actor);
		if (IsValid(enemy) && enemy->ActorHasTag(TAG_ENEMY))
		{
			// 에너미 이동속도 변경 요청
			enemy->EnemyStunLogin(StunTime);
		}
	}

	// 이펙트 활성화
	ActivateStunEffect(ParticleComponent);
}

// 이펙트 활성화 비활성화
void ACommonCharacter_DarkSwordMan::ActivateStunEffect(UParticleSystemComponent* PSystem)
{
	if (!IsActivatePaticle)
	{
		IsActivatePaticle = true;
		ParticleComponent->Activate();
	}
	else
	{
		IsActivatePaticle = false;
		ParticleComponent->Deactivate();
	}
}

// 평타 기반 범위 스턴 시키는 스킬
bool ACommonCharacter_DarkSwordMan::SkillBtn01()
{
	StunEnemyLogic();
	return false;
}

// 상위 유닛 조합 스킬
bool ACommonCharacter_DarkSwordMan::SkillBtn05()
{
	
	return false;
}
