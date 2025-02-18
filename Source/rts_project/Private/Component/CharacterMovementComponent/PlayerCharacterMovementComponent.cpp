// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CharacterMovementComponent/PlayerCharacterMovementComponent.h"
#include "Controller/PlayerCharacter/GamePlayerController.h"
#include "Actor/PlayerCharacter/GamePlayerCharacter.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "../rts_project.h"


UPlayerCharacterMovementComponent::UPlayerCharacterMovementComponent()
{
	GamePlayer = Cast<AGamePlayerCharacter>(GetOwner());

	MoveSpeed = 30.0f;
	currentX = currentY = 0.0f;
}

void UPlayerCharacterMovementComponent::OnInputMouseX(float axis)
{
	currentX = axis; // 현재 값을 입력
	FVector nowActorLocation = GamePlayer->GetActorLocation();
	FRotator ControllerRotation = FRotator(0.0f, GamePlayer->GetControlRotation().Yaw, 0.0f);
	FVector controllerRightVector = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::Y);
		
	GamePlayer->SetActorLocation(nowActorLocation + (controllerRightVector * axis * MoveSpeed));
}

void UPlayerCharacterMovementComponent::OnInputMouseY(float axis)
{
	currentY = axis; // 현재 값을 입력
	FVector nowActorLocation = GamePlayer->GetActorLocation();
	FRotator ControllerRotation = FRotator(0.0f, GamePlayer->GetControlRotation().Yaw, 0.0f);
	FVector controllerForwardVector = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
		
	GamePlayer->SetActorLocation(nowActorLocation + (controllerForwardVector * axis * -1 * MoveSpeed));
}
