#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FriendlyUnitAttackComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UFriendlyUnitAttackComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	// 무기 정보를 담는 DataTable Asset
	UPROPERTY()
	class UDataTable* WeaponInfoDataTable;

	// 이 컴포넌트를 가진 액터유닛
	UPROPERTY()
	class AFriendlyAbleCharacterBase* PlayerUnit;

	// 장착중인 무기
	UPROPERTY()
	class AWeaponBase* EquippedWeapon;


public:	
	UFriendlyUnitAttackComponentBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 초기화
	virtual void OnBeginPlayInitialize(class AFriendlyAbleCharacterBase* ownerPlayerUnit);

	// 공격 요청
	virtual bool OnAttackRequested(FName weaponCode);

	// 공격이 끝났을 때 호출되는 함수
	virtual void OnAttackFinished();

	// 무기 장착 함수
	class AWeaponBase* EquipWeapon(
		FName weaponCode,
		USceneComponent* attachTo,
		FTransform transform = FTransform(),
		FName socketName = FName(TEXT("Socket_Weapon")));

	// 착용중인 무기를 초기화한다.
	void DeleteWeapon();

private:
	// 무기 코드를 이용해 무기 정보를 가져옴
	const struct FWeaponInfoTableRow* GetWeaponInfo(const FName& weaponCode) const;
	
private:
	// 공격 영역 검사 시작됨
	UFUNCTION()
	void CALLBACK_OnAttackAreaCheckStarted();

	// 공격 영역 검사 종료됨
	UFUNCTION()
	void CALLBACK_OnAttackAreaCheckFinished();

	// 공격중 새로운 액터 감지됨
	UFUNCTION()
	void CALLBACK_OnAttackDetected(class AActor* newDetected);
};
