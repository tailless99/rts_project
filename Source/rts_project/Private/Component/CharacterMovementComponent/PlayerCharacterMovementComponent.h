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
	// ���콺 ��ġ�� ���� ������ ����
	void OnInputMouseX(float axis);
	void OnInputMouseY(float axis);

	// ���콺 ��ġ Ȯ��
	FVector2D EdgeScroll();
};
