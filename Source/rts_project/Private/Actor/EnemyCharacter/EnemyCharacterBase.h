// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacterBase.generated.h"



UCLASS()
class AEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* BodyComponent;

	UPROPERTY()
	class UDataTable* EnemyDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defensive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Reward;

	UPROPERTY()
	class UWidgetComponent* WidgetComponent;

	UPROPERTY()
	class UEnemyHpPercentBarWidget* EnemyHpPercentBar;

public:
	FName EnemyCode;
	FTimerHandle stunTimer;

private:
	void OnEnemyDead();

public:
	AEnemyCharacterBase();

	virtual void InitializeEnemyInfo(class AEnemyControllerBase* enemyController, struct FEnemyDataTableRow* enemyInfo);
	

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* enemyController) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// 이동속도 감소 이벤트 시작
	void CALLBACK_EnemySpeedDownEventStarted(float newSpeed);

	// 이동속도 감소 이벤트 끝
	void CALLBACK_EnemySpeedDownEventEnded();

	// 스턴 로직
	void EnemyStunLogin(float stunTime);
};
