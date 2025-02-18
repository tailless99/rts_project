// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/MainUI/BottomWidget/BottomWidgetContainer.h"
#include "Main_Container_Widget.generated.h"

/**
 * 
 */
UCLASS()
class UMain_Container_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
	class UTopWidgetContainer* BP_TopContainerWidget;

	UPROPERTY(Meta = (BindWidget))
	class UMidWidgetContainer* BP_MidContainerWidget;

	UPROPERTY(Meta = (BindWidget))
	class UBottomWidgetContainer* BP_BottomContainerWidget;

	UPROPERTY(Meta = (BindWidget))
	class USkillBtnInfoWidget* BP_SkillBtnInfo;

	UPROPERTY(Meta = (BindWidget))
	class UGameOverWidget* BP_GameOverWidget;

	UPROPERTY(Meta = (BindWidget))
	class UOverlay* GameOverPanel;

public:
	UMain_Container_Widget(const FObjectInitializer& ObjectInitializer);
	void InitializeWidget();

	void GameOver();
	void GameWin();

public:
	void ShowSkillInfoTextBox(FName SkillName, FString SKillBtnInfo);
	void UnShowSkillInfoTextBox();

public:
	FORCEINLINE UTopWidgetContainer* GetTopWidgetContainer() { return BP_TopContainerWidget; }
	FORCEINLINE UMidWidgetContainer* GetMidWidgetContainer() { return BP_MidContainerWidget; }
	FORCEINLINE UBottomWidgetContainer* GetBottomWidgetContainer() { return BP_BottomContainerWidget; }
};
