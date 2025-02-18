#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum/SkillBtnType.h"
#include "Actor/UseableUnit/FriendlyCharacter/FriendlyAbleCharacterBase.h"
#include "BottomWidgetContainer.generated.h"





UCLASS()
class UBottomWidgetContainer : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY()
	class ARtsMarqueeHUD* HudComponent;

	UPROPERTY()
	class UDataTable* unitInfo;

	UPROPERTY(Meta = (BindWidget))
	class UBorder* MultiSelect_Border;

	UPROPERTY(Meta = (BindWidget))
	class UUniformGridPanel* MultiSelect_Box;
	
	UPROPERTY(Meta = (BindWidget))
	class UBorder* UnitInfo_Border;	

	UPROPERTY(Meta = (BindWidget))
	class UImage* MainUnitImg;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* MainUnitHp;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* MainUnitMp;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* AttackPoint;

	UPROPERTY(Meta = (BindWidget))
	class UVerticalBox* Defence_Info_Box;

	UPROPERTY(Meta = (BindWidget))
	class UBottomButtonWidget* BP_BottomButtonWidget_Attack;

	UPROPERTY(Meta = (BindWidget))
	class UBottomButtonWidget* BP_BottomButtonWidget_Hold;

	UPROPERTY(Meta = (BindWidget))
	class UBottomButtonWidget* BP_BottomButtonWidget_Petroll;

	UPROPERTY(Meta = (BindWidget))
	class UBottomButtonWidget* BP_BottomButtonWidget_Sell;

	UPROPERTY(Meta = (BindWidget))
	class UBottomButtonWidget* BP_BottomButtonWidget01;

	UPROPERTY(Meta = (BindWidget))
	class UBottomButtonWidget* BP_BottomButtonWidget02;

	UPROPERTY(Meta = (BindWidget))
	class UBottomButtonWidget* BP_BottomButtonWidget03;

	UPROPERTY(Meta = (BindWidget))
	class UBottomButtonWidget* BP_BottomButtonWidget04;

	UPROPERTY(Meta = (BindWidget))
	class UBottomButtonWidget* BP_BottomButtonWidget05;

	UPROPERTY(Meta = (BindWidget))
	class UBottomButtonWidget* BP_BottomButtonWidget06;

	UPROPERTY(Meta = (BindWidget))
	class UBottomButtonWidget* BP_BottomButtonWidget07;

	UPROPERTY(Meta = (BindWidget))
	class UBottomButtonWidget* BP_BottomButtonWidget08;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* MultiUnitBtn01;

	UPROPERTY(Meta = (BindWidget))
	class UButton* MultiUnitBtn02;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* MultiUnitBtn03;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* MultiUnitBtn04;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* MultiUnitBtn05;

	UPROPERTY(Meta = (BindWidget))
	class UButton* MultiUnitBtn06;

	UPROPERTY(Meta = (BindWidget))
	class UButton* MultiUnitBtn07;

	UPROPERTY(Meta = (BindWidget))
	class UButton* MultiUnitBtn08;

	UPROPERTY(Meta = (BindWidget))
	class UButton* MultiUnitBtn09;

	UPROPERTY(Meta = (BindWidget))
	class UButton* MultiUnitBtn10;

	UPROPERTY(Meta = (BindWidget))
	class UButton* MultiUnitBtn11;

	UPROPERTY(Meta = (BindWidget))
	class UButton* MultiUnitBtn12;

	UPROPERTY()
	class AGamePlayerController* playerController;
	

	UTexture2D* SkillBlackBackground;
	UTexture2D* AttackBtnImgTexture;
	UTexture2D* HoldBtnImgTexture;
	UTexture2D* PetrollBtnImgTexture;
	UTexture2D* SellBtnImgTexture;
		
	FVector TargetLocation;
	AActor* TargetActor;

	bool IsUpdateSelectUnitsArray;

private:
	TArray<AFriendlyAbleCharacterBase*> SelectUnits;
	TArray<AFriendlyAbleCharacterBase*> CanActionUnits;

	// 스킬 버튼들의 초기화등을 편리하게 하기 위해 저장해놓는 변수
	TArray<UBottomButtonWidget*> SkilBtns;

public:
	void InitializeWidget();
	UBottomWidgetContainer(const FObjectInitializer& ObjectInitializer);

public:
	void SetHudComponent(ARtsMarqueeHUD* hudComponent);
	void SetUnitInfo();

private:
	void UpdateSelectUnit();
	void choiceSkillButton(SkillBtnType skillType);
	void SetUnitSkillBtnLink(FFriendlyUnitDataTableRow* selectUnitInfo);
	void InitializeUISkillImg();
	void CanActionUnitCheck();
	
public:
	// 공통 사용 스킬이 클릭됬을 때 출력되는 함수
	void OnClickCommonSkillButtonAction(SkillBtnType btnType);

	// 스킬 버튼이 호버됬을 때 실행되는 함수
	void OnSkillBtnHover(int32 btnCode);

	// 스킬 버튼이 호버됬을 때 실행되는 함수
	void OnSkillBtnUnHover();

	// 버튼 위젯에서 사용하는 버튼을 리스트에 등록하는 함수
	void AddBtnArray(UBottomButtonWidget* myBtnWidget);

	// 데이터 테이블에서 정보를 가져오는 함수
	FFriendlyUnitDataTableRow* GetUnitDataTableRow(FName unitCode);

	// 리스트 요소 제거
	void RemoveArrayAt(AFriendlyAbleCharacterBase* removeActor);

	// 클릭된 액터를 찾는다
	void FindSelectActorToMouseClick();

	// 버튼이 클릭됬을 때 실행될 함수
	void OnClickButtonAction(SkillBtnType btnType);

public:
	// 멀티 유닛 박스의 공동 사용 함수
	UFUNCTION()
	void CALLBACK_MultiUnitBoxSelect(UButton* selectMultiUnitBtn);

	UFUNCTION()
	void CALLBACK_MultiUnitBtn01();

	UFUNCTION()
	void CALLBACK_MultiUnitBtn02();

	UFUNCTION()
	void CALLBACK_MultiUnitBtn03();

	UFUNCTION()
	void CALLBACK_MultiUnitBtn04();

	UFUNCTION()
	void CALLBACK_MultiUnitBtn05();

	UFUNCTION()
	void CALLBACK_MultiUnitBtn06();

	UFUNCTION()
	void CALLBACK_MultiUnitBtn07();

	UFUNCTION()
	void CALLBACK_MultiUnitBtn08();

	UFUNCTION()
	void CALLBACK_MultiUnitBtn09();

	UFUNCTION()
	void CALLBACK_MultiUnitBtn10();

	UFUNCTION()
	void CALLBACK_MultiUnitBtn11();

	UFUNCTION()
	void CALLBACK_MultiUnitBtn12();


	FORCEINLINE TArray<AFriendlyAbleCharacterBase*> GetSelectUnitsList() { return SelectUnits; }
};
