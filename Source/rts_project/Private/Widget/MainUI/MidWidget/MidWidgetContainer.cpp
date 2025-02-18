#include "Widget/MainUI/MidWidget/MidWidgetContainer.h"
#include "Actor/LocationActor/WispSpawnLocation.h"
#include "Actor/PlayerCharacter/GamePlayerCharacter.h"
#include "Actor/UseableUnit/FriendlyCharacter/Wisp/WispUnit.h"
#include "Components/Button.h"
#include "EngineUtils.h"
#include "Controller/PlayerCharacter/GamePlayerController.h"
#include "rts_project/rts_project.h"



void UMidWidgetContainer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// ������ �ʵ忡 �����ϴ��� ã��, �ִٸ� ��ư�� Ȱ��ȭ
	SerchWisp();
}

void UMidWidgetContainer::InitializeWidget()
{
	WispBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_ShoWWispLocation);

	// ���� ���� ã��
	SerchStartPointLocations();
}

// ���� ������ ã���ϴ�
void UMidWidgetContainer::SerchStartPointLocations()
{
	for (TActorIterator<AActorSpawnLocationBase> It(GetWorld()); It; ++It) {
		AActorSpawnLocationBase* Actor = *It;

		if (AWispSpawnLocation* wispSpawnLocation = Cast<AWispSpawnLocation>(Actor))
			GoStartPointLocation = wispSpawnLocation->GetActorLocation() + 
				(FVector::UpVector * 1592.0f + FVector::BackwardVector * 500.0f);
	}
}

// �ʵ忡 ������ �����ϴ��� ã���ϴ�.
void UMidWidgetContainer::SerchWisp()
{	
	bool bWispFound = false;

	for (TActorIterator<AWispUnit> It(GetWorld()); It; ++It) {
		if (AWispUnit* wispUnit = Cast<AWispUnit>(*It)) {
			bWispFound = true;
			break;
		}
	}
	WispBtn->SetVisibility(bWispFound ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

// ī�޶� ������ ��ŸƮ �������� ������
void UMidWidgetContainer::CALLBACK_ShoWWispLocation()
{
	AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	AGamePlayerCharacter* GamePlayer = playerController->GamePlayer;

	// Ŭ����, ������������ ���� �̵�
	GamePlayer->SetActorLocation(GoStartPointLocation);
}
