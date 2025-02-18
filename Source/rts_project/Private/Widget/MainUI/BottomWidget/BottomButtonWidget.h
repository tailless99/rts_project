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
	// ��ų ��ư�� Ÿ���� �����ϴ� ����
	SkillBtnType skillBtnType;

private:
	// ��û�� ���� ��, ��ȿ�� ��ų�� ����ϴ��� üũ�ϴ� �Լ��� ��Ƴ��� �Լ�
	bool RequestCheckFunc();

	// ���� �÷��̾� ��Ʈ�ѷ����� Ÿ���� üũ �Ǿ����� Ȯ���ϴ� ����
	bool CheckTargetLocation();

public:
	UBottomButtonWidget(const FObjectInitializer& ObjectInitializer);

	// ��ų ��ư �ʱ�ȭ
	void InitializeWidget();

	// ���� ��ų ��ư �ʱ�ȭ
	void InitializeWidget_CommonSkill();

	// �ش� ������ ��ų�� ����ϴ��� �Ǵ��ϴ� �Լ�
	bool IsUseSkilCheck(FFriendlyUnitDataTableRow* unitDataInfo);

public: // ���� ��� ��ų ���� �ۺ� �Լ�
	// ���� ��ų�� Ŭ������ �� ��µǴ� �Լ�
	UFUNCTION()
	void CALLBACK_OnClickCommonSKillFunc();


public: // Skill01~08 ���� �ۺ� �Լ�
	// Ŭ������ �� ��µǴ� �Լ�
	UFUNCTION()
	void CALLBACK_OnClickFunc();

	// ȣ������ �� ��µǴ� �Լ�
	UFUNCTION()
	void CALLBACK_OnHoverFunc();

	// ȣ�� �ƿ���� �� ��µǴ� �Լ�
	UFUNCTION()
	void CALLBACK_OnUnHoverFunc();

public:
	UImage* GetChildImg();
};
