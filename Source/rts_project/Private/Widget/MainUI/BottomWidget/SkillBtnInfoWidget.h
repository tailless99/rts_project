// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillBtnInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class USkillBtnInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	class UTextBlock* unitSkillName;

	UPROPERTY()
	class UTextBlock* unitSkillInfo;

public:
	USkillBtnInfoWidget(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION()
	void SetSkillName(FName skillText);

	UFUNCTION()
	void SetSkillInfo(FString skillText);
};
