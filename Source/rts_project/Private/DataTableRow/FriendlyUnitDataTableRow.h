// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enum/RarityTypeEnum.h"
#include "FriendlyUnitDataTableRow.generated.h"


USTRUCT(BlueprintType)
struct FSkill_Info {
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsUseSkill = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Skill_ToolTip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Skill_Img;
};

USTRUCT(BlueprintType)
struct FFriendlyUnitDataTableRow final : public FTableRowBase
{
	GENERATED_BODY();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERarityTypeEnum Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AFriendlyAbleCharacterBase> UseCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class TSubclassOf<class AFriendlyUnitController> UseControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UFriendlyAbleUnitAnimInstance> UseAnimInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* UseBehavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* CharacterImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinAttackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAttackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UnitMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SellPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkill_Info SkillBtn01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkill_Info SkillBtn02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkill_Info SkillBtn03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkill_Info SkillBtn04;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkill_Info SkillBtn05;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkill_Info SkillBtn06;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkill_Info SkillBtn07;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkill_Info SkillBtn08;
};
