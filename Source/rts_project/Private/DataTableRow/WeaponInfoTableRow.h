#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponInfoTableRow.generated.h"


USTRUCT(BlueprintType)
struct FWeaponInfoTableRow final : public FTableRowBase
{
	GENERATED_BODY()

public:
	// ���� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWeaponBase> WeaponClass;

	// ���� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText WeaponName;

	// ���⿡ ����� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* UseAttackAnimMontage;

public:
	FWeaponInfoTableRow();
};
