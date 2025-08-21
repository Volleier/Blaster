#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "SpeedPickup.generated.h"

/**
 * 速度拾取类（ASpeedPickup）
 * 继承自 APickup，用于实现角色获得速度加成的拾取物。
 */
UCLASS()
class BLASTER_API ASpeedPickup : public APickup
{
	GENERATED_BODY()

protected:
	/**
	 * 当球体碰撞体发生重叠时调用
	 * @param OverlappedComponent 发生重叠的组件
	 * @param OtherActor 参与重叠的其他Actor
	 * @param OtherComp 参与重叠的其他组件
	 * @param OtherBodyIndex 其他组件的Body索引
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
	/** 角色获得的基础速度加成（单位：cm/s） */
	UPROPERTY(EditAnywhere)
	float BaseSpeedBuff = 1600.f;

	/** 角色蹲下时获得的速度加成（单位：cm/s） */
	UPROPERTY(EditAnywhere)
	float CrouchSpeedBuff = 850.f;

	/** 速度加成持续时间（单位：秒） */
	UPROPERTY(EditAnywhere)
	float SpeedBuffTime = 30.f;
};
