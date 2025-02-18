// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/GameTitle/GameTitlePlayerController.h"
#include "Widget/GameTitle/GameTitleSecWidget.h"


AGameTitlePlayerController::AGameTitlePlayerController()
{
	static ConstructorHelpers::FClassFinder<UGameTitleSecWidget> WIDGET_GAMETITLE(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/GameTitle/WidgetBP_GameTitleSec.WidgetBP_GameTitleSec_C'"));

	if (WIDGET_GAMETITLE.Succeeded())
	{
		WidgetBP_GameTitleSec = WIDGET_GAMETITLE.Class;
	}
}

void AGameTitlePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	GameTitleSec = CreateWidget<UGameTitleSecWidget>(this, WidgetBP_GameTitleSec);
	GameTitleSec->AddToViewport();
	GameTitleSec->InitializeWidget();
}

