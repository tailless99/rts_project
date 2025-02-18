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
	SpringArmComponent->TargetArmLength = 150.0f;	// 암 길이 설정
	//SpringArmComponent->SetRelativeRotation(FRotator(-65.0f, 0.0f, 0.0f));

	GameCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GAME_CAMERA_COMP"));
	GameCamera->SetRelativeRotation(FRotator(-65.0f, 0.0f, 0.0f));	// 카메라 각도 설정
	GameCamera->SetupAttachment(SpringArmComponent);

	// 액터의 높이 설정
	GetRootComponent()->SetRelativeLocation(FVector(0.0f, 0.0f, 1500.0f));
		
	// 중력 영향을 받지 않도록 설정
	movement = Cast<UPlayerCharacterMovementComponent>(GetCharacterMovement());
	movement->GravityScale = 0;
	movement->BrakingDecelerationWalking = 2000.0f; // 높은 값으로 설정하여 빨리 멈추도록 함
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
