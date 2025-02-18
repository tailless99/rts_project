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
	// 공격 영역을 감지 및 레이캐스트 쏘기
	void CheckAttackArea();
};
