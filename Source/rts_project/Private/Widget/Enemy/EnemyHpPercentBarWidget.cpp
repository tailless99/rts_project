#include "Widget/Enemy/EnemyHpPercentBarWidget.h"
#include "Components/ProgressBar.h"

void UEnemyHpPercentBarWidget::InitializeWidget()
{
	// �ۼ�Ʈ �ʱ�ȭ
	EnemyHpBar->SetPercent(1.0f);
}

// hp �ۼ�Ʈ ����
void UEnemyHpPercentBarWidget::SetEnemyHpPercent(float currentHpPercent)
{
	EnemyHpBar->SetPercent(currentHpPercent);
}
