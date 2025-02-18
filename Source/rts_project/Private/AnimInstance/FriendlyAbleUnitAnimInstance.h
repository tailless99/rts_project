// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FriendlyAbleUnitAnimInstance.generated.h"


// ���� ���� �̺�Ʈ ���� ����
DECLARE_EVENT(UEnemyCharacterAnimInstanceBase, FOnAttackFinishEventSignature)

// ���� ���� �˻� ���� �̺�Ʈ
DECLARE_EVENT(UEnemyCharacterAnimInstanceBase, FOnAttackAreaCheckStartEventSignature)

// ���� ���� �˻� �� �̺�Ʈ
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
	// ���� �� �̺�Ʈ
	FOnAttackFinishEventSignature OnAttackFinished;

	// ���� ���� �˻� ���� �̺�Ʈ
	FOnAttackAreaCheckStartEventSignature OnAttackAreaCheckStarted;
	// ���� ���� �˻� �� �̺�Ʈ
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
	// �ʱ�ȭ �Լ�
	void OnInitializeAinmInstance(class AFriendlyAbleCharacterBase* playerUnit);

	// �ӵ� ������Ʈ
	void OnUpdateSpeed(float NewSpeed);

	// ���ݻ��� ����
	void OnAttack();

private:
	// ������ ������ �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void AnimNotify_OnAttackFinished();


	// ���� ���� �˻� ����
	UFUNCTION()
	void AnimNotify_StartAttackAreaCheck();

	// ���� ���� �˻� ��
	UFUNCTION()
	void AnimNotify_FinishAttackAreaCheck();

};
