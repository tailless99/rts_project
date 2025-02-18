#include "Actor/PlayerCharacter/GamePlayerCharacter.h"
#include "Component/SpringArmComponent/GamePlayerSpringArmComponent.h"
#include "Component/CharacterMovementComponent/PlayerCharacterMovementComponent.h"
#include "Controller/PlayerCharacter/GamePlayerController.h"
#include "Camera/CameraComponent.h"
#include "rts_project/rts_project.h"

AGamePlayerCharacter::AGamePlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<UGamePlayerSpringArmComponent>(TEXT("SPRING_ARM_COMP"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 150.0f;	// �� ���� ����
	//SpringArmComponent->SetRelativeRotation(FRotator(-65.0f, 0.0f, 0.0f));

	GameCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GAME_CAMERA_COMP"));
	GameCamera->SetRelativeRotation(FRotator(-65.0f, 0.0f, 0.0f));	// ī�޶� ���� ����
	GameCamera->SetupAttachment(SpringArmComponent);

	// ������ ���� ����
	GetRootComponent()->SetRelativeLocation(FVector(0.0f, 0.0f, 1500.0f));
		
	// �߷� ������ ���� �ʵ��� ����
	movement = Cast<UPlayerCharacterMovementComponent>(GetCharacterMovement());
	movement->GravityScale = 0;
	movement->BrakingDecelerationWalking = 2000.0f; // ���� ������ �����Ͽ� ���� ���ߵ��� ��
}


void AGamePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}


void AGamePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGamePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGamePlayerCharacter::OnInputMouseX(float axis)
{
	movement->OnInputMouseX(axis);
}

void AGamePlayerCharacter::OnInputMouseY(float axis)
{
	movement->OnInputMouseY(axis);
}

FVector AGamePlayerCharacter::GetPlayerForwardVector()
{
	return SpringArmComponent->GetForwardVector();
}

FVector AGamePlayerCharacter::GetPlayerRightVector()
{
	return SpringArmComponent->GetRightVector();
}
