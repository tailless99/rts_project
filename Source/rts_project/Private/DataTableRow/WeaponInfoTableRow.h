#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponInfoTableRow.generated.h"


USTRUCT(BlueprintType)
struct FWeaponInfoTableRow final : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 무기 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWeaponBase> WeaponClass;

	// 무기 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText WeaponName;

	// 무기에 사용할 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* UseAttackAnimMontage;

public:
	FWeaponInfoTableRow();
};
