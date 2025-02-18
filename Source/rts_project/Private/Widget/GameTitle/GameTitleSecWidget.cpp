// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/GameTitle/GameTitleSecWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UGameTitleSecWidget::InitializeWidget()
{
	Btn_GoMainGame->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_GoMainGame);
}

void UGameTitleSecWidget::CALLBACK_GoMainGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("RtsGameLevel"));
}
