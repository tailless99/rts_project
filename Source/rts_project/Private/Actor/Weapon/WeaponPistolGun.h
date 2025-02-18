// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon/WeaponBase.h"
#include "WeaponPistolGun.generated.h"

/**
 * 
 */
UCLASS()
class AWeaponPistolGun : public AWeaponBase
{
	GENERATED_BODY()

private:
	/*UPROPERTY()
		class */

public:
	AWeaponPistolGun();
	virtual void Tick(float DeltaTime) override;

public:
	// ���� ������ ���� �� ����ĳ��Ʈ ���
	void CheckAttackArea();
};
