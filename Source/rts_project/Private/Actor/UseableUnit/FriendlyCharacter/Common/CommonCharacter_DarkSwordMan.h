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
	// 스턴 범위
	float StunRadius;

	// 스턴 시간
	float StunTime;

	bool IsActivatePaticle;

public:
	ACommonCharacter_DarkSwordMan();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// 공격이 끝날 경우 호출되는 함수
	virtual void OnAttackFinished() override;

private:
	// 범위 내의 적을 스턴시킨다
	void StunEnemyLogic();

	// 파티클 활성화/비활성화
	void ActivateStunEffect(UParticleSystemComponent* PSystem);

public:
	// 인터페이스 함수 상속
	// 평타 기반으로 범위의 적을 스턴 시키는 스킬
	virtual bool SkillBtn01() override;

	// 상위 유닛 조합 스킬
	virtual bool SkillBtn05() override;	
};
