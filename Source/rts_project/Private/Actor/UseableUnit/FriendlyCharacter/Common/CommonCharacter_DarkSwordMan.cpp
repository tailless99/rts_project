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


	// ȸ�� ����
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);

	// ���� ������Ʈ ����
	AttackComponent = CreateDefaultSubobject<UFriendlyUnitAttackComponentBase>(TEXT("ATTACK_COMP"));
	
	// ���� �ڵ� ����
	UnitCode = TEXT("0004");

	// ��� ���� �ڵ� ����
	UseWeaponCode = TEXT("0003");	

	// ���� ����
	StunRadius = 300.f;

	// ���� �ð�
	StunTime = 1.f;

	// ��ƼŬ �ý��� ��Ȱ��ȭ
	IsActivatePaticle = false;
}

void ACommonCharacter_DarkSwordMan::BeginPlay()
{
	Super::BeginPlay();

	// ���� ����
	FTransform weaponSocketTransform = GetMesh()->GetSocketTransform(TEXT("Socket_Weapon_DarkSword"));
	AttackComponent->EquipWeapon(UseWeaponCode, GetMesh(), weaponSocketTransform, TEXT("Socket_Weapon_DarkSword"));

	// ��ƼŬ �ý����� ������ �ڵ����� ����
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

	// ��Ÿ ������ ������ ��, Ȯ���� ���� ��ų ����
	// �ߵ� Ȯ�� 90�ۼ�Ʈ
	if (random < 101)
	{
		SkillBtn01();
	}
}

// ���� ���� ����
void ACommonCharacter_DarkSwordMan::StunEnemyLogic()
{
	FVector UnitLocation = GetActorLocation();

	TArray<AActor*> actorsToIgnore{ this };
	TArray<AActor*> OverlappingActors;

	UKismetSystemLibrary::SphereOverlapActors(
		this, UnitLocation, StunRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>>(), nullptr,
		actorsToIgnore, OverlappingActors);

	// �������� ���� ���� ��� �߰�
	for (AActor* actor : OverlappingActors)
	{
		AEnemyCharacterBase* enemy = Cast<AEnemyCharacterBase>(actor);
		if (IsValid(enemy) && enemy->ActorHasTag(TAG_ENEMY))
		{
			// ���ʹ� �̵��ӵ� ���� ��û
			enemy->EnemyStunLogin(StunTime);
		}
	}

	// ����Ʈ Ȱ��ȭ
	ActivateStunEffect(ParticleComponent);
}

// ����Ʈ Ȱ��ȭ ��Ȱ��ȭ
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

// ��Ÿ ��� ���� ���� ��Ű�� ��ų
bool ACommonCharacter_DarkSwordMan::SkillBtn01()
{
	StunEnemyLogic();
	return false;
}

// ���� ���� ���� ��ų
bool ACommonCharacter_DarkSwordMan::SkillBtn05()
{
	
	return false;
}
