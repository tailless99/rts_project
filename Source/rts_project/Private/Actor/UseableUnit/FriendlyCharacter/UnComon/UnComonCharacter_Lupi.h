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
	AUnComonCharacter_Lupi();

	virtual void BeginPlay() override;

public:
	// �����, ���� ��ȯ ��ġ�� ������
	void SetMovePointLocation();

public:
	// �������̽� �Լ� ���
	// ��Ŀ�� ��޿����� ��ų�� ������� ������ ���� ��Ȱ��ȭ ó��
	virtual bool SkillBtn01() override;

	// ���� ���� ���� ��ų
	virtual bool SkillBtn05() override;

	/*virtual bool SkillBtn02() override;
	virtual bool SkillBtn03() override;
	virtual bool SkillBtn04() override;
	virtual bool SkillBtn05() override;
	virtual bool SkillBtn06() override;
	virtual bool SkillBtn07() override;
	virtual bool SkillBtn08() override;*/
};