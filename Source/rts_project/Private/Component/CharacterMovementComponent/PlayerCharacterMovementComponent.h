// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class UPlayerCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MoveSpeed;

protected:
	float currentX;
	float currentY;

public:
	UPlayerCharacterMovementComponent();

protected:
	UPROPERTY()
	class AGamePlayerCharacter* GamePlayer;

public:
	// 마우스 위치에 따른 움직임 구현
	void OnInputMouseX(float axis);
	void OnInputMouseY(float axis);

	// 마우스 위치 확인
	FVector2D EdgeScroll();
};
