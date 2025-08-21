#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "AmmoPickup.generated.h"

/**
 * AAmmoPickup 类用于表示弹药拾取物。
 * 继承自 APickup，玩家角色可以通过与其碰撞来获得弹药。
 */
UCLASS()
class BLASTER_API AAmmoPickup : public APickup
{
	GENERATED_BODY()

protected:
	/**
	 * 当球形碰撞体发生重叠时调用。
	 * @param OverlappedComponent 发生重叠的组件
	 * @param OtherActor 与拾取物发生重叠的其他Actor
	 * @param OtherComp 与拾取物发生重叠的其他组件
	 * @param OtherBodyIndex 其他组件的索引
	 * @param bFromSweep 是否为扫掠检测
	 * @param SweepResult 扫掠检测结果
	 */
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	/** 弹药数量，默认值为30，可在编辑器中设置 */
	UPROPERTY(EditAnywhere)
	int32 AmmoAmount = 30;

	/** 弹药对应的武器类型，可在编辑器中设置 */
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
};