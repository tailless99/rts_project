#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon/WeaponBase.h"
#include "WeaponSword.generated.h"


UCLASS()
class AWeaponSword : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AWeaponSword();

	virtual void Tick(float DeltaTime) override;

public:
	// 공격 영역을 감지
	void CheckAttackArea();

	// 공격 영역 검사를 위해 끝점을 계산
	void UpdateAttackAreaCheckPoints(FVector& out_start, FVector& out_end);

};
