#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


// 공격 영역 감지 이벤트
DECLARE_EVENT_OneParam(AWeaponBase, OnAttackAreaDetectEventSignature, AActor*)



UCLASS()
class AWeaponBase : public AActor
{
	GENERATED_BODY()
	
private:
	// 이 무기에 대한 정보
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
	// 공격 감지시 발생하는 이벤트
	OnAttackAreaDetectEventSignature OnDetected;

protected:
	// 공격 영역을 감지하는 시작점에 해당하는 소켓 이름
	FName AttackAreaCheckStartSocketName;

	// 공격 영역을 감지하는 끝점에 해당하는 소켓 이름
	FName AttackAreaCheckEndSocketName;

	// 공격 영역에서 감지한 액터들
	TArray<AActor*> DetectedActors;
	
public:	
	AWeaponBase();

	// 무기 정보를 설정
	virtual void SetWeaponInfo(const struct FWeaponInfoTableRow* weaponInfo);

protected:
	virtual void BeginPlay() override;
	
	// 공격 영역 감지
	//virtual void CheckAttackArea();

public:	
	virtual void Tick(float DeltaTime) override;

	// 공격을 시작할 때 호출
	virtual void OnAttackStarted();

	// 공격이 끝날 때 호출
	virtual void OnAttackFinished();	

	// 공격 영역 활성화, 비활성화 설정
	virtual void EnableAttackArea(bool bEnable);

	// 적의 정보를 받아옵니다.
	void SetTarget(AActor* Enemy);

public:
	// 무기 정보를 반환
	FORCEINLINE const struct FWeaponInfoTableRow* GetWeaponInfo() const { return WeaponInfo; }

};
