#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileBullet.generated.h"

/**
 * AProjectileBullet 类
 * 继承自 AProjectile，用于实现子弹类型的投射物。
 */
UCLASS()
class BLASTER_API AProjectileBullet : public AProjectile
{
	GENERATED_BODY()

public:
	// 构造函数
	AProjectileBullet();

protected:
	/**
	 * 当投射物碰撞时调用的函数
	 * @param HitComp      发生碰撞的组件
	 * @param OtherActor   被碰撞的其他 Actor
	 * @param OtherComp    被碰撞的其他组件
	 * @param NormalImpulse 碰撞产生的冲量
	 * @param Hit          碰撞结果信息
	 */
	virtual void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit) override;

private:
	// 私有成员变量（如有需要可在此添加）
};