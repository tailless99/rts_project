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

	// 위습이 필드에 존재하는지 찾고, 있다면 버튼을 활성화
	SerchWisp();
}

void UMidWidgetContainer::InitializeWidget()
{
	WispBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::CALLBACK_ShoWWispLocation);

	// 시작 지점 찾기
	SerchStartPointLocations();
}

// 시작 지점을 찾습니다
void UMidWidgetContainer::SerchStartPointLocations()
{
	for (TActorIterator<AActorSpawnLocationBase> It(GetWorld()); It; ++It) {
		AActorSpawnLocationBase* Actor = *It;

		if (AWispSpawnLocation* wispSpawnLocation = Cast<AWispSpawnLocation>(Actor))
			GoStartPointLocation = wispSpawnLocation->GetActorLocation() + 
				(FVector::UpVector * 1592.0f + FVector::BackwardVector * 500.0f);
	}
}

// 필드에 위습이 존재하는지 찾습니다.
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

// 카메라 시점을 스타트 지점으로 돌린다
void UMidWidgetContainer::CALLBACK_ShoWWispLocation()
{
	AGamePlayerController* playerController = Cast<AGamePlayerController>(GetWorld()->GetFirstPlayerController());
	AGamePlayerCharacter* GamePlayer = playerController->GamePlayer;

	// 클릭시, 시작지점으로 시점 이동
	GamePlayer->SetActorLocation(GoStartPointLocation);
}
