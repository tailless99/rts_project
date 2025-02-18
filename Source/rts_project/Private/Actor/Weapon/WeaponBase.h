#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


// ���� ���� ���� �̺�Ʈ
DECLARE_EVENT_OneParam(AWeaponBase, OnAttackAreaDetectEventSignature, AActor*)



UCLASS()
class AWeaponBase : public AActor
{
	GENERATED_BODY()
	
private:
	// �� ���⿡ ���� ����
	const struct FWeaponInfoTableRow* WeaponInfo;

public:
	UPROPERTY()
	class USceneComponent* DefaultWeaponRoot;

	UPROPERTY()
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY()
	class AEnemyCharacterBase* TargetEnemy;

	UPROPERTY()
	bool bAttackAreaEnable;

public:
	// ���� ������ �߻��ϴ� �̺�Ʈ
	OnAttackAreaDetectEventSignature OnDetected;

protected:
	// ���� ������ �����ϴ� �������� �ش��ϴ� ���� �̸�
	FName AttackAreaCheckStartSocketName;

	// ���� ������ �����ϴ� ������ �ش��ϴ� ���� �̸�
	FName AttackAreaCheckEndSocketName;

	// ���� �������� ������ ���͵�
	TArray<AActor*> DetectedActors;
	
public:	
	AWeaponBase();

	// ���� ������ ����
	virtual void SetWeaponInfo(const struct FWeaponInfoTableRow* weaponInfo);

protected:
	virtual void BeginPlay() override;
	
	// ���� ���� ����
	//virtual void CheckAttackArea();

public:	
	virtual void Tick(float DeltaTime) override;

	// ������ ������ �� ȣ��
	virtual void OnAttackStarted();

	// ������ ���� �� ȣ��
	virtual void OnAttackFinished();	

	// ���� ���� Ȱ��ȭ, ��Ȱ��ȭ ����
	virtual void EnableAttackArea(bool bEnable);

	// ���� ������ �޾ƿɴϴ�.
	void SetTarget(AActor* Enemy);

public:
	// ���� ������ ��ȯ
	FORCEINLINE const struct FWeaponInfoTableRow* GetWeaponInfo() const { return WeaponInfo; }

};
