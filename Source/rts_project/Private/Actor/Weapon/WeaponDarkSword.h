// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon/WeaponBase.h"
#include "WeaponDarkSword.generated.h"

/**
 * 
 */
UCLASS()
class AWeaponDarkSword : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AWeaponDarkSword();
	virtual void Tick(float DeltaTime) override;
		
public:
	// ���� ������ ����
	void CheckAttackArea();

	// ���� ���� �˻縦 ���� ������ ���
	void UpdateAttackAreaCheckPoints(FVector& out_start, FVector& out_end);
};
