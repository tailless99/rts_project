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
	// ���� ������ ����
	void CheckAttackArea();

	// ���� ���� �˻縦 ���� ������ ���
	void UpdateAttackAreaCheckPoints(FVector& out_start, FVector& out_end);

};
