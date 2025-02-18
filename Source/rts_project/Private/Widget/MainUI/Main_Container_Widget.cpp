#include "Widget/MainUI/Main_Container_Widget.h"
#include "Widget/MainUI/TopWidget/TopWidgetContainer.h"
#include "Widget/MainUI/MidWidget/MidWidgetContainer.h"
#include "Widget/MainUI/BottomWidget/BottomWidgetContainer.h"
#include "Widget/MainUI/BottomWidget/SkillBtnInfoWidget.h"
#include "Widget/MainUI/GameOverWidget.h"
#include "Controller/PlayerCharacter/GamePlayerController.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanelSlot.h"
#include "HUD/RtsMarqueeHUD.h"
#include "rts_project/rts_project.h"




UMain_Container_Widget::UMain_Container_Widget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void UMain_Container_Widget::InitializeWidget()
{
	BP_TopContainerWidget->InitializeWidget();
	BP_MidContainerWidget->InitializeWidget();
	BP_BottomContainerWidget->InitializeWidget();
	BP_GameOverWidget->InitializeWidget();

	BP_SkillBtnInfo->SetVisibility(ESlateVisibility::Collapsed);
	GameOverPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UMain_Container_Widget::GameOver()
{
	GetWorld()->GetWorldSettings()->SetTimeDilation(0);
	
	// 위젯 타이틀 텍스트 문구 설정
	BP_GameOverWidget->SetGameStateText(TEXT("Game Over"));

	// 게임오버 창 보이도록 수정
	GameOverPanel->SetVisibility(ESlateVisibility::Visible);
}

void UMain_Container_Widget::GameWin()
{
	GetWorld()->GetWorldSettings()->SetTimeDilation(0);
	
	// 위젯 타이틀 텍스트 문구 설정
	BP_GameOverWidget->SetGameStateText(TEXT("Game Win"));

	// 게임오버 창 보이도록 수정
	GameOverPanel->SetVisibility(ESlateVisibility::Visible);
}

void UMain_Container_Widget::ShowSkillInfoTextBox(FName SkillName, FString SKillBtnInfo)
{
	BP_SkillBtnInfo->SetSkillName(SkillName);
	BP_SkillBtnInfo->SetSkillInfo(SKillBtnInfo);
	BP_SkillBtnInfo->SetVisibility(ESlateVisibility::Visible);
}

void UMain_Container_Widget::UnShowSkillInfoTextBox()
{
	BP_SkillBtnInfo->SetVisibility(ESlateVisibility::Collapsed);
}
