// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GamePlayerCharacter.generated.h"

UCLASS()
class AGamePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UGamePlayerSpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* GameCamera;

	class UPlayerCharacterMovementComponent* movement;

public:
	AGamePlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void OnInputMouseX(float axis);
	void OnInputMouseY(float axis);

public:
	FVector GetPlayerForwardVector();
	FVector GetPlayerRightVector();
};
