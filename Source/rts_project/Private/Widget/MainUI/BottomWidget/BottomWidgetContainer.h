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

	// ��ų ��ư���� �ʱ�ȭ���� ���ϰ� �ϱ� ���� �����س��� ����
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
	// ���� ��� ��ų�� Ŭ������ �� ��µǴ� �Լ�
	void OnClickCommonSkillButtonAction(SkillBtnType btnType);

	// ��ų ��ư�� ȣ������ �� ����Ǵ� �Լ�
	void OnSkillBtnHover(int32 btnCode);

	// ��ų ��ư�� ȣ������ �� ����Ǵ� �Լ�
	void OnSkillBtnUnHover();

	// ��ư �������� ����ϴ� ��ư�� ����Ʈ�� ����ϴ� �Լ�
	void AddBtnArray(UBottomButtonWidget* myBtnWidget);

	// ������ ���̺��� ������ �������� �Լ�
	FFriendlyUnitDataTableRow* GetUnitDataTableRow(FName unitCode);

	// ����Ʈ ��� ����
	void RemoveArrayAt(AFriendlyAbleCharacterBase* removeActor);

	// Ŭ���� ���͸� ã�´�
	void FindSelectActorToMouseClick();

	// ��ư�� Ŭ������ �� ����� �Լ�
	void OnClickButtonAction(SkillBtnType btnType);

public:
	// ��Ƽ ���� �ڽ��� ���� ��� �Լ�
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
