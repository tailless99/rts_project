// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "CommonCharacter_DarkSwordMan.generated.h"

/**
 * 
 */
UCLASS()
class ACommonCharacter_DarkSwordMan : public AFriendlyAbleCharacterBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UParticleSystemComponent* ParticleComponent;

private:
	// ���� ����
	float StunRadius;

	// ���� �ð�
	float StunTime;

	bool IsActivatePaticle;

public:
	ACommonCharacter_DarkSwordMan();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// ������ ���� ��� ȣ��Ǵ� �Լ�
	virtual void OnAttackFinished() override;

private:
	// ���� ���� ���� ���Ͻ�Ų��
	void StunEnemyLogic();

	// ��ƼŬ Ȱ��ȭ/��Ȱ��ȭ
	void ActivateStunEffect(UParticleSystemComponent* PSystem);

public:
	// �������̽� �Լ� ���
	// ��Ÿ ������� ������ ���� ���� ��Ű�� ��ų
	virtual bool SkillBtn01() override;

	// ���� ���� ���� ��ų
	virtual bool SkillBtn05() override;	
};
