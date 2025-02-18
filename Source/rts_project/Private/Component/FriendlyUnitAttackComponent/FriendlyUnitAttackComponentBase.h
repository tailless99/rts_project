#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FriendlyUnitAttackComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UFriendlyUnitAttackComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	// ���� ������ ��� DataTable Asset
	UPROPERTY()
	class UDataTable* WeaponInfoDataTable;

	// �� ������Ʈ�� ���� ��������
	UPROPERTY()
	class AFriendlyAbleCharacterBase* PlayerUnit;

	// �������� ����
	UPROPERTY()
	class AWeaponBase* EquippedWeapon;


public:	
	UFriendlyUnitAttackComponentBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// �ʱ�ȭ
	virtual void OnBeginPlayInitialize(class AFriendlyAbleCharacterBase* ownerPlayerUnit);

	// ���� ��û
	virtual bool OnAttackRequested(FName weaponCode);

	// ������ ������ �� ȣ��Ǵ� �Լ�
	virtual void OnAttackFinished();

	// ���� ���� �Լ�
	class AWeaponBase* EquipWeapon(
		FName weaponCode,
		USceneComponent* attachTo,
		FTransform transform = FTransform(),
		FName socketName = FName(TEXT("Socket_Weapon")));

	// �������� ���⸦ �ʱ�ȭ�Ѵ�.
	void DeleteWeapon();

private:
	// ���� �ڵ带 �̿��� ���� ������ ������
	const struct FWeaponInfoTableRow* GetWeaponInfo(const FName& weaponCode) const;
	
private:
	// ���� ���� �˻� ���۵�
	UFUNCTION()
	void CALLBACK_OnAttackAreaCheckStarted();

	// ���� ���� �˻� �����
	UFUNCTION()
	void CALLBACK_OnAttackAreaCheckFinished();

	// ������ ���ο� ���� ������
	UFUNCTION()
	void CALLBACK_OnAttackDetected(class AActor* newDetected);
};
