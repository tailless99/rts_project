// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FriendlyAbleUnitAnimInstance.generated.h"


// 공격 끝남 이벤트 형식 선언
DECLARE_EVENT(UEnemyCharacterAnimInstanceBase, FOnAttackFinishEventSignature)

// 공격 영역 검사 시작 이벤트
DECLARE_EVENT(UEnemyCharacterAnimInstanceBase, FOnAttackAreaCheckStartEventSignature)

// 공격 영역 검사 끝 이벤트
DECLARE_EVENT(UEnemyCharacterAnimInstanceBase, FOnAttackAreaCheckFinishEventSignature)



UCLASS()
class UFriendlyAbleUnitAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	class AFriendlyAbleCharacterBase* PlayerUnit;

public:
	UPROPERTY()
	class UAnimMontage* CurrentMontage;

public:
	// 공격 끝 이벤트
	FOnAttackFinishEventSignature OnAttackFinished;

	// 공격 영역 검사 시작 이벤트
	FOnAttackAreaCheckStartEventSignature OnAttackAreaCheckStarted;
	// 공격 영역 검사 끝 이벤트
	FOnAttackAreaCheckFinishEventSignature OnAttackAreaCheckFinished;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsAttack;

public:
	UFriendlyAbleUnitAnimInstance();
	void PlayAnimMontage(class UAnimMontage* montageAsset);
	void StopAnimMontage();

public:
	// 초기화 함수
	void OnInitializeAinmInstance(class AFriendlyAbleCharacterBase* playerUnit);

	// 속도 업데이트
	void OnUpdateSpeed(float NewSpeed);

	// 공격상태 변경
	void OnAttack();

private:
	// 공격이 끝났을 때 호출되는 함수
	UFUNCTION()
	void AnimNotify_OnAttackFinished();


	// 공격 영역 검사 시작
	UFUNCTION()
	void AnimNotify_StartAttackAreaCheck();

	// 공격 영역 검사 끝
	UFUNCTION()
	void AnimNotify_FinishAttackAreaCheck();

};
