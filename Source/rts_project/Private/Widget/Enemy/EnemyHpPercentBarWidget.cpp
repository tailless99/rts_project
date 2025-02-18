#include "Widget/Enemy/EnemyHpPercentBarWidget.h"
#include "Components/ProgressBar.h"

void UEnemyHpPercentBarWidget::InitializeWidget()
{
	// 퍼센트 초기화
	EnemyHpBar->SetPercent(1.0f);
}

// hp 퍼센트 변경
void UEnemyHpPercentBarWidget::SetEnemyHpPercent(float currentHpPercent)
{
	EnemyHpBar->SetPercent(currentHpPercent);
}
