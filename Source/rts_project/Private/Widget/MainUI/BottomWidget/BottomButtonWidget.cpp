#include "Widget/MainUI/BottomWidget/BottomButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "DataTableRow/FriendlyUnitDataTableRow.h"
#include "Controller/PlayerCharacter/GamePlayerController.h"
#include "Widget/MainUI/Main_Container_Widget.h"
#include "Widget/MainUI/BottomWidget/BottomWidgetContainer.h"
#include "Widget/MainUI/BottomWidget/SkillBtnInfoWidget.h"
#include "Enum/SkillBtnType.h"
#include "Enum/MouseLeftBtnActionType.h"
#include "rts_project/rts_project.h"




#define WISP_UNITCODE TEXT("0003")


UBottomButtonWidget::UBottomButtonWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBottomButtonWidget::InitializeWidget()
{
	playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	BottomWidgetContainer = playerController->MainContainerWidget->GetBottomWidgetContainer();

	BottomBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_OnClickFunc);
	BottomBtn->OnHovered.AddUniqueDynamic(this, &ThisClass::CALLBACK_OnHoverFunc);
	BottomBtn->OnUnhovered.AddUniqueDynamic(this, &ThisClass::CALLBACK_OnUnHoverFunc);

	// ��ư�� ����Ʈ�� ���
	BottomWidgetContainer->AddBtnArray(this);
}

void UBottomButtonWidget::InitializeWidget_CommonSkill()
{
	playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	BottomWidgetContainer = playerController->MainContainerWidget->GetBottomWidgetContainer();

	BottomBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_OnClickCommonSKillFunc);
	BottomBtn->OnHovered.AddUniqueDynamic(this, &ThisClass::CALLBACK_OnHoverFunc);
	BottomBtn->OnUnhovered.AddUniqueDynamic(this, &ThisClass::CALLBACK_OnUnHoverFunc);
	
	// ��ư�� ����Ʈ�� ���
	BottomWidgetContainer->AddBtnArray(this);
}

// ��ư �ڵ带 �����Ͽ� �ش� ��ư�� ��ų�� ���ֿ��� ���Ǵ��� �Ǵ��ϴ� �Լ�
bool UBottomButtonWidget::IsUseSkilCheck(FFriendlyUnitDataTableRow* unitDataInfo)
{
	bool IsUseBtn = false;

	switch (ButtonCode)
	{
	case 1:
		IsUseBtn = unitDataInfo->SkillBtn01.IsUseSkill;
		skillBtnType = SkillBtnType::skill01;
		break;
	case 2:
		IsUseBtn = unitDataInfo->SkillBtn02.IsUseSkill;
		skillBtnType = SkillBtnType::skill02;
		break;
	case 3:
		IsUseBtn = unitDataInfo->SkillBtn03.IsUseSkill;
		skillBtnType = SkillBtnType::skill03;
		break;
	case 4:
		IsUseBtn = unitDataInfo->SkillBtn04.IsUseSkill;
		skillBtnType = SkillBtnType::skill04;
		break;
	case 5:
		IsUseBtn = unitDataInfo->SkillBtn05.IsUseSkill;
		skillBtnType = SkillBtnType::skill05;
		break;
	case 6:
		IsUseBtn = unitDataInfo->SkillBtn06.IsUseSkill;
		skillBtnType = SkillBtnType::skill06;
		break;
	case 7:
		IsUseBtn = unitDataInfo->SkillBtn07.IsUseSkill;
		skillBtnType = SkillBtnType::skill07;
		break;
	case 8:
		IsUseBtn = unitDataInfo->SkillBtn08.IsUseSkill;
		skillBtnType = SkillBtnType::skill08;
		break;

	// ���� ��ų
	case 9:	// ����
		IsUseBtn = true;
		skillBtnType = SkillBtnType::Attack;
		break;

	case 10:// Ȧ��
		IsUseBtn = true;
		skillBtnType = SkillBtnType::Hold;
		break;

	case 11:// ����
		IsUseBtn = true;
		skillBtnType = SkillBtnType::Petroll;
		break;

	case 12:// �Ǹ�
		IsUseBtn = true;
		skillBtnType = SkillBtnType::Sell;
		break;
	}
	return IsUseBtn;
}

// ��û�� ���� ��, ��ȿ�� ��ų�� ����ϴ��� üũ�ϴ� �Լ��� ��Ƴ��� �Լ�
bool UBottomButtonWidget::RequestCheckFunc()
{
	// ���õ� ���� ����Ʈ�� �����´�
	TArray<AFriendlyAbleCharacterBase*> SelectUnits = BottomWidgetContainer->GetSelectUnitsList();

	if (SelectUnits.Num() <= 0) return false;
	if (SelectUnits[0]->UnitCode == WISP_UNITCODE) return false;

	// ����Ʈ ���� ���� ĳ���� �ϳ��� ��ų ����
	FFriendlyUnitDataTableRow* unitDataInfo = BottomWidgetContainer->GetUnitDataTableRow(SelectUnits[0]->UnitCode);

	// ��ư �ڵ忡 �ش��ϴ� ��ų�� ������ ����ϴ��� Ȯ���ϱ� ���� ����
	bool IsUseBtn = false;

	// ��ȿ���� ������ ��ȯ
	if (!unitDataInfo) return false;

	IsUseBtn = IsUseSkilCheck(unitDataInfo);

	// ������� �ʴ� ��ų�̸� ��ȯ
	if (!IsUseBtn) return false;

	return true;
}

bool UBottomButtonWidget::CheckTargetLocation()
{
	if (playerController->MouseLeftBtnActionType == EMouseLeftBtnActionType::None)
	{
		// ���� ��û
		if (ButtonCode == 9)
		{
			SetUserFocus(playerController);
			playerController->CALLBACK_OnAttackBtnPressed();
			return false;
		}

		// ���� ��û
		if(ButtonCode == 11)
		{
			SetUserFocus(playerController);
			playerController->CALLBACK_OnPetrollBtnPressed();
			return false;
		}
	}
	return true;
}

// Ŭ���� ����Ǵ� �Լ�
void UBottomButtonWidget::CALLBACK_OnClickFunc()
{	
	bool RequestCheck = RequestCheckFunc();
	if (!RequestCheck) return;

	BottomWidgetContainer->OnClickButtonAction(skillBtnType);

}

// ���� ��� �Լ��� Ŭ������ �� ���Ǵ� �Լ�
void UBottomButtonWidget::CALLBACK_OnClickCommonSKillFunc()
{
	bool RequestCheck = RequestCheckFunc();
	if (!RequestCheck) return;

	// ����, ������ Ÿ���� �ִ��� Ȯ��
	if (!CheckTargetLocation()) return;

	BottomWidgetContainer->OnClickCommonSkillButtonAction(skillBtnType);
}

// ��ư ȣ�� �� ����Ǵ� �Լ�
void UBottomButtonWidget::CALLBACK_OnHoverFunc()
{
	bool RequestCheck = RequestCheckFunc();
	if (!RequestCheck) return;
	BottomWidgetContainer->OnSkillBtnHover(ButtonCode);
}

// ȣ�� �ƿ���� �� ��µǴ� �Լ�
void UBottomButtonWidget::CALLBACK_OnUnHoverFunc()
{
	BottomWidgetContainer->OnSkillBtnUnHover();
}

UImage* UBottomButtonWidget::GetChildImg()
{
	return Cast<UImage>(BottomBtn->GetChildAt(0));
}
