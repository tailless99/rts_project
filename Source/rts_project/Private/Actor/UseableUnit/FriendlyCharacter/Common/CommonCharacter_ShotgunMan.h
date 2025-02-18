#pragma once

#include "CoreMinimal.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "CommonCharacter_ShotgunMan.generated.h"

/**
 * 
 */
UCLASS()
class ACommonCharacter_ShotgunMan : public AFriendlyAbleCharacterBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		float FindEnemyAreaRadius;

	UPROPERTY()
		float DebuffSpeedDown;

	UPROPERTY()
		bool IsUseSpeedDownSkill;

	TArray<class AEnemyCharacterBase*> DetectedEnemys;

public:
	ACommonCharacter_ShotgunMan();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	// ���� 1�� ��ų �������� �������� �����ϴ� �Լ�
	void CheckEnemyDetectedArea();

public:
	// �������̽� �Լ� ���
	// ������ ���� �̵��ӵ��� ���ҽ�Ű�� ��ų
	virtual bool SkillBtn01() override;

	// ���� ���� ���� ��ų
	virtual bool SkillBtn05() override;
};
