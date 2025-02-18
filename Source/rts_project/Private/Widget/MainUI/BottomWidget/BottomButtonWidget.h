// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTableRow/FriendlyUnitDataTableRow.h"
#include "Components/Image.h"
#include "Enum/SkillBtnType.h"
#include "BottomButtonWidget.generated.h"



UCLASS()
class UBottomButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta = (BindWidget))
	class UButton* BottomBtn;
	
	UPROPERTY()
	class AGamePlayerController* playerController;

	UPROPERTY()
	class UBottomWidgetContainer* BottomWidgetContainer;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 ButtonCode;

private:
	// 스킬 버튼의 타입을 저장하는 변수
	SkillBtnType skillBtnType;

private:
	// 요청이 왔을 때, 유효한 스킬을 사용하는지 체크하는 함수를 모아놓은 함수
	bool RequestCheckFunc();

	// 게임 플레이어 컨트롤러에서 타겟이 체크 되었는지 확인하는 구문
	bool CheckTargetLocation();

public:
	UBottomButtonWidget(const FObjectInitializer& ObjectInitializer);

	// 스킬 버튼 초기화
	void InitializeWidget();

	// 공통 스킬 버튼 초기화
	void InitializeWidget_CommonSkill();

	// 해당 유닛의 스킬이 사용하는지 판단하는 함수
	bool IsUseSkilCheck(FFriendlyUnitDataTableRow* unitDataInfo);

public: // 공통 사용 스킬 전용 퍼블릭 함수
	// 공통 스킬이 클릭됬을 때 출력되는 함수
	UFUNCTION()
	void CALLBACK_OnClickCommonSKillFunc();


public: // Skill01~08 전용 퍼블릭 함수
	// 클릭됬을 때 출력되는 함수
	UFUNCTION()
	void CALLBACK_OnClickFunc();

	// 호버됬을 때 출력되는 함수
	UFUNCTION()
	void CALLBACK_OnHoverFunc();

	// 호버 아웃됬을 때 출력되는 함수
	UFUNCTION()
	void CALLBACK_OnUnHoverFunc();

public:
	UImage* GetChildImg();
};
