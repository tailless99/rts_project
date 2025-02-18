#include "Widget/MainUI/TopWidget/TopWidgetContainer.h"
#include "Components/TextBlock.h"



UTopWidgetContainer::UTopWidgetContainer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UTopWidgetContainer::InitializeWidget()
{
	// ÃÊ±âÈ­
	Gold_Text->SetText(FText::FromString(TEXT("0")));
	Wood_Text->SetText(FText::FromString(TEXT("0")));
	Timer_Text->SetText(FText::FromString(TEXT("00")));
	EnemyCount_Text->SetText(FText::FromString(TEXT("00")));
	RoundText->SetText(FText::FromString(TEXT("00")));
}

void UTopWidgetContainer::UpdateEnemyReward(float reward)
{
	float temp = FCString::Atof(*Gold_Text->GetText().ToString());
	temp += reward;
	Gold_Text->SetText(FText::AsNumber(temp));
}

void UTopWidgetContainer::UpdateWaveReward(float reward)
{
	float temp = FCString::Atof(*Wood_Text->GetText().ToString());
	temp += reward;
	Wood_Text->SetText(FText::AsNumber(temp));
}

void UTopWidgetContainer::UpdateTimer()
{
	float temp = FCString::Atof(*Timer_Text->GetText().ToString());
	temp -= 1;
	Timer_Text->SetText(FText::AsNumber(temp));
}

void UTopWidgetContainer::SetTimer(float time)
{
	Timer_Text->SetText(FText::AsNumber(time));
}

void UTopWidgetContainer::SetRound(float round)
{
	RoundText->SetText(FText::AsNumber(round));
}

float UTopWidgetContainer::GetGold()
{
	FString TextValue = Gold_Text->GetText().ToString();
	float FloatValue = FCString::Atof(*TextValue);
	return FloatValue;
}

float UTopWidgetContainer::GetWood()
{
	FString TextValue = Wood_Text->GetText().ToString();
	float FloatValue = FCString::Atof(*TextValue);
	return FloatValue;
}

void UTopWidgetContainer::UpdateEnemyCount(float addCount)
{
	float temp = FCString::Atof(*EnemyCount_Text->GetText().ToString());
	temp += addCount;
	EnemyCount_Text->SetText(FText::AsNumber(temp));
}
