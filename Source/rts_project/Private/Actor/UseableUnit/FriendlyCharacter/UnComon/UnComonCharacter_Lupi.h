// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "UnComonCharacter_Lupi.generated.h"

/**
 * 
 */
UCLASS()
class AUnComonCharacter_Lupi : public AFriendlyAbleCharacterBase
{
	GENERATED_BODY()
	
public:
	// 창고 시작 위치
	UPROPERTY()
	FVector StorageStartLocation;

	// 창고 끝 위치
	UPROPERTY()
	FVector StorageEndLocation;

	// 라인 소환 위치
	UPROPERTY()
	FVector UnitSpawnLocation;

	// 창고로 이동했는지 여부
	bool IsMoveStorage;

public:
	AUnComonCharacter_Lupi();

	virtual void BeginPlay() override;

public:
	// 저장소, 라인 소환 위치를 셋팅함
	void SetMovePointLocation();

public:
	// 인터페이스 함수 상속
	// 언커먼 등급에서는 스킬을 사용하지 않으니 전부 비활성화 처리
	virtual bool SkillBtn01() override;

	// 상위 유닛 조합 스킬
	virtual bool SkillBtn05() override;

	/*virtual bool SkillBtn02() override;
	virtual bool SkillBtn03() override;
	virtual bool SkillBtn04() override;
	virtual bool SkillBtn05() override;
	virtual bool SkillBtn06() override;
	virtual bool SkillBtn07() override;
	virtual bool SkillBtn08() override;*/
};