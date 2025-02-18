#include "Widget/MainUI/BottomWidget/SkillBtnInfoWidget.h"
#include "Components/TextBlock.h"





USkillBtnInfoWidget::USkillBtnInfoWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void USkillBtnInfoWidget::SetSkillName(FName skillName)
{
	unitSkillName->SetText(FText::FromString(skillName.ToString()));
}

void USkillBtnInfoWidget::SetSkillInfo(FString skillInfo)
{
	unitSkillInfo->SetText(FText::FromString(skillInfo));
}
