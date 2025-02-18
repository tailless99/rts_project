// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillFuncInterface.generated.h"
/**
 * 
 */
UINTERFACE(BlueprintType, NotBlueprintable)
class USkillFuncInterface : public UInterface
{
    GENERATED_BODY()
};

class ISkillFuncInterface
{
    GENERATED_BODY()

public:
	virtual bool SkillBtn01() = 0;
	virtual bool SkillBtn02() = 0;
	virtual bool SkillBtn03() = 0;
	virtual bool SkillBtn04() = 0;
	virtual bool SkillBtn05() = 0;
	virtual bool SkillBtn06() = 0;
	virtual bool SkillBtn07() = 0;
	virtual bool SkillBtn08() = 0;
};