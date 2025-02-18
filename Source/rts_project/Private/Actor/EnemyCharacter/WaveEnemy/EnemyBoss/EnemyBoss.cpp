#include "Actor/EnemyCharacter/WaveEnemy/EnemyBoss/EnemyBoss.h"
#include "Controller/Enemy/WaveEnemy/WaveEnemyController.h"


AEnemyBoss::AEnemyBoss()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_SKELETON(
		TEXT(""));

	if (SK_SKELETON.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_SKELETON.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	EnemyCode = TEXT("0002");
	AIControllerClass = AWaveEnemyController::StaticClass();
}
