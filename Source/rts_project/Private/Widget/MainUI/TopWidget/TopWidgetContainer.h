// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopWidgetContainer.generated.h"

/**
 * 
 */
UCLASS()
class UTopWidgetContainer : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* Gold_Text;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* Wood_Text;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* Timer_Text;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* RoundText;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* EnemyCount_Text;


public:
	UTopWidgetContainer(const FObjectInitializer& ObjectInitializer);
	void InitializeWidget();

	void UpdateEnemyReward(float reward);
	void UpdateWaveReward(float reward);
	void UpdateEnemyCount(float addCount);
	void UpdateTimer();
	void SetTimer(float time);
	void SetRound(float round);

public:
	// ∞ÒµÂ ¿‹∑Æ π›»Ø
	float GetGold();
	
	// ∏Ò¿Á ¿‹∑Æ π›»Ø
	float GetWood();
};
