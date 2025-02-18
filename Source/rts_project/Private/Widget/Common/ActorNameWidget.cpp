#include "Widget/Common/ActorNameWidget.h"
#include "Components/TextBlock.h"
#include "rts_project/rts_project.h"

void UActorNameWidget::InitializeWidget(FName actorName)
{
	ActorNameTextBlock->SetText(FText::FromName(actorName));
}
