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
	// 적이 1번 스킬 범위내로 들어오는지 감지하는 함수
	void CheckEnemyDetectedArea();

public:
	// 인터페이스 함수 상속
	// 범위내 적의 이동속도를 감소시키는 스킬
	virtual bool SkillBtn01() override;

	// 상위 유닛 조합 스킬
	virtual bool SkillBtn05() override;
};
