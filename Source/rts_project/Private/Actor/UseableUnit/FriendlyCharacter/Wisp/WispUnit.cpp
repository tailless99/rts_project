// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/UseableUnit/FriendlyCharacter/Wisp/WispUnit.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "rts_project/rts_project.h"


AWispUnit::AWispUnit()
{
	Wisp_Body = GetCapsuleComponent();

	// 회전 설정
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);

	Tags.Empty();
	Tags.Add(TAG_WISP);
}


void AWispUnit::BeginPlay()
{
	Super::BeginPlay();
		
	GetMesh()->SetCollisionProfileName(COLLISIONNAME_NOCOLLISION);
}

void AWispUnit::DestroyWisp()
{
	this->Destroy();
}
