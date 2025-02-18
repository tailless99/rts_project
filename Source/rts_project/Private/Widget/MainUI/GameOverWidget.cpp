#include "Widget/MainUI/GameOverWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "rts_project/rts_project.h"
#include "kismet/GameplayStatics.h"


void UGameOverWidget::InitializeWidget()
{
	RetryBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_RetryGame);
	TitleBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_GoTitle);
}

void UGameOverWidget::CALLBACK_RetryGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("RtsGameLevel"));
}

void UGameOverWidget::CALLBACK_GoTitle()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("GameTitle"));
}

void UGameOverWidget::SetGameStateText(FName gameStateText)
{
	GameStateText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *gameStateText.ToString())));
}
