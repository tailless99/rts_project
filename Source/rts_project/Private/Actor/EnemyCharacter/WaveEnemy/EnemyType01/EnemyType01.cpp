// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EnemyCharacter/WaveEnemy/EnemyType01/EnemyType01.h"
#include "Controller/Enemy/WaveEnemy/WaveEnemyController.h"

AEnemyType01::AEnemyType01()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_SKELETON(
		TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/SKnight_modular/Skeleton_Knight_01/mesh/SK_SKnigh_01_full.SK_SKnigh_01_full'"));

	if (SK_SKELETON.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_SKELETON.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	EnemyCode = TEXT("0001");
	AIControllerClass = AWaveEnemyController::StaticClass();
}
