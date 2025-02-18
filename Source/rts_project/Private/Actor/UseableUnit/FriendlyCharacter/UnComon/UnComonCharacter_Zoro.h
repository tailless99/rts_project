// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "UnComonCharacter_Zoro.generated.h"

/**
 * 
 */
UCLASS()
class AUnComonCharacter_Zoro : public AFriendlyAbleCharacterBase
{
	GENERATED_BODY()

public:
	// â�� ���� ��ġ
	UPROPERTY()
		FVector StorageStartLocation;

	// â�� �� ��ġ
	UPROPERTY()
		FVector StorageEndLocation;

	// ���� ��ȯ ��ġ
	UPROPERTY()
		FVector UnitSpawnLocation;

	// â��� �̵��ߴ��� ����
	bool IsMoveStorage;

public:
	AUnComonCharacter_Zoro();
	
	virtual void BeginPlay() override;

public:
	// �����, ���� ��ȯ ��ġ�� ������
	void SetMovePointLocation();

public:
	// â��� �̵� ��ų
	virtual bool SkillBtn01() override;

	// ���� ���� ���� ��ų
	virtual bool SkillBtn05() override;
};
