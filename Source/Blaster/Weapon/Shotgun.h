#pragma once

#include "CoreMinimal.h"
#include "HitScanWeapon.h"
#include "Shotgun.generated.h"

/**
 * AShotgun 类
 * 继承自 AHitScanWeapon，表示霰弹枪武器类型。
 * 该类实现了霰弹枪的发射逻辑，并包含用于设置弹丸数量的属性。
 */
UCLASS()
class BLASTER_API AShotgun : public AHitScanWeapon
{
	GENERATED_BODY()

public:
	/**
	 * 重写 Fire 方法
	 * @param HitTarget 命中的目标位置
	 * 实现霰弹枪的发射逻辑。
	 */
	virtual void FireShotgun(const TArray<FVector_NetQuantize>& HitTargets);

    /**
    * 计算霰弹枪弹丸的散射终点位置
    * @param HitTarget 玩家瞄准的目标位置
    * @param HitTargets 存储每个弹丸实际命中的位置
    * 根据霰弹枪的弹丸数量和散射逻辑，生成每个弹丸的实际命中点。
    */
	void ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets);

private:
	/**
	 * 弹丸数量
	 * 用于设置每次射击发射的弹丸数，默认为 10。
	 * 可在编辑器中通过 "Weapon Scatter" 分类进行调整。
	 */
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	uint32 NumberOfPellets = 10;
};
