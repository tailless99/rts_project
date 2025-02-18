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

	// 버튼을 리스트에 등록
	BottomWidgetContainer->AddBtnArray(this);
}

void UBottomButtonWidget::InitializeWidget_CommonSkill()
{
	playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	BottomWidgetContainer = playerController->MainContainerWidget->GetBottomWidgetContainer();

	BottomBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_OnClickCommonSKillFunc);
	BottomBtn->OnHovered.AddUniqueDynamic(this, &ThisClass::CALLBACK_OnHoverFunc);
	BottomBtn->OnUnhovered.AddUniqueDynamic(this, &ThisClass::CALLBACK_OnUnHoverFunc);
	
	// 버튼을 리스트에 등록
	BottomWidgetContainer->AddBtnArray(this);
}

// 버튼 코드를 참조하여 해당 버튼의 스킬이 유닛에서 사용되는지 판단하는 함수
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

	// 공통 스킬
	case 9:	// 공격
		IsUseBtn = true;
		skillBtnType = SkillBtnType::Attack;
		break;

	case 10:// 홀드
		IsUseBtn = true;
		skillBtnType = SkillBtnType::Hold;
		break;

	case 11:// 정찰
		IsUseBtn = true;
		skillBtnType = SkillBtnType::Petroll;
		break;

	case 12:// 판매
		IsUseBtn = true;
		skillBtnType = SkillBtnType::Sell;
		break;
	}
	return IsUseBtn;
}

// 요청이 왔을 때, 유효한 스킬을 사용하는지 체크하는 함수를 모아놓은 함수
bool UBottomButtonWidget::RequestCheckFunc()
{
	// 선택된 유닛 리스트를 가져온다
	TArray<AFriendlyAbleCharacterBase*> SelectUnits = BottomWidgetContainer->GetSelectUnitsList();

	if (SelectUnits.Num() <= 0) return false;
	if (SelectUnits[0]->UnitCode == WISP_UNITCODE) return false;

	// 리스트 가장 앞의 캐릭터 하나만 스킬 실행
	FFriendlyUnitDataTableRow* unitDataInfo = BottomWidgetContainer->GetUnitDataTableRow(SelectUnits[0]->UnitCode);

	// 버튼 코드에 해당하는 스킬을 유닛이 사용하는지 확인하기 위한 변수
	bool IsUseBtn = false;

	// 유효하지 않으면 반환
	if (!unitDataInfo) return false;

	IsUseBtn = IsUseSkilCheck(unitDataInfo);

	// 사용하지 않는 스킬이면 반환
	if (!IsUseBtn) return false;

	return true;
}

bool UBottomButtonWidget::CheckTargetLocation()
{
	if (playerController->MouseLeftBtnActionType == EMouseLeftBtnActionType::None)
	{
		// 공격 요청
		if (ButtonCode == 9)
		{
			SetUserFocus(playerController);
			playerController->CALLBACK_OnAttackBtnPressed();
			return false;
		}

		// 정찰 요청
		if(ButtonCode == 11)
		{
			SetUserFocus(playerController);
			playerController->CALLBACK_OnPetrollBtnPressed();
			return false;
		}
	}
	return true;
}

// 클릭시 실행되는 함수
void UBottomButtonWidget::CALLBACK_OnClickFunc()
{	
	bool RequestCheck = RequestCheckFunc();
	if (!RequestCheck) return;

	BottomWidgetContainer->OnClickButtonAction(skillBtnType);

}

// 공통 사용 함수가 클릭됬을 때 사용되는 함수
void UBottomButtonWidget::CALLBACK_OnClickCommonSKillFunc()
{
	bool RequestCheck = RequestCheckFunc();
	if (!RequestCheck) return;

	// 공격, 정찰의 타겟이 있는지 확인
	if (!CheckTargetLocation()) return;

	BottomWidgetContainer->OnClickCommonSkillButtonAction(skillBtnType);
}

// 버튼 호버 시 실행되는 함수
void UBottomButtonWidget::CALLBACK_OnHoverFunc()
{
	bool RequestCheck = RequestCheckFunc();
	if (!RequestCheck) return;
	BottomWidgetContainer->OnSkillBtnHover(ButtonCode);
}

// 호버 아웃됬을 때 출력되는 함수
void UBottomButtonWidget::CALLBACK_OnUnHoverFunc()
{
	BottomWidgetContainer->OnSkillBtnUnHover();
}

UImage* UBottomButtonWidget::GetChildImg()
{
	return Cast<UImage>(BottomBtn->GetChildAt(0));
}
