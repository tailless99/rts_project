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
	// 공격 영역을 감지
	void CheckAttackArea();

	// 공격 영역 검사를 위해 끝점을 계산
	void UpdateAttackAreaCheckPoints(FVector& out_start, FVector& out_end);
};
