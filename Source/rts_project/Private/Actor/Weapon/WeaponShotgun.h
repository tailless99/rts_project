// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon/WeaponBase.h"
#include "WeaponShotgun.generated.h"

/**
 * 
 */
UCLASS()
class AWeaponShotgun : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AWeaponShotgun();
	virtual void Tick(float DeltaTime) override;

public:
	// ���� ������ ���� �� ����ĳ��Ʈ ���
	void CheckAttackArea();
};
