#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHpPercentBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class UEnemyHpPercentBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta = (BindWidget))
	class UProgressBar* EnemyHpBar;

public:
	void InitializeWidget();

	// hp �ۼ�Ʈ ����
	void SetEnemyHpPercent(float currentHpPercent);
};
