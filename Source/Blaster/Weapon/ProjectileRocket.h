#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"

/**
 * 火箭弹投射物类
 * 继承自AProjectile，表示游戏中的火箭弹投射物。
 */
UCLASS()
class BLASTER_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()

public:
	// 构造函数
	AProjectileRocket();

	// 当投射物被销毁时调用
	virtual void Destroyed() override;

protected:
	// 当投射物碰撞时调用
	virtual void OnHit(
		UPrimitiveComponent* HitComp,	// 被击中的组件
		AActor* OtherActor,				// 其他参与碰撞的Actor
		UPrimitiveComponent* OtherComp, // 其他参与碰撞的组件
		FVector NormalImpulse,			// 碰撞产生的冲量
		const FHitResult& Hit			// 碰撞结果
	) override;

	// 投射物生成时调用
	virtual void BeginPlay() override;

	// 投射物循环音效
	UPROPERTY(EditAnywhere)
	USoundCue* ProjectileLoop;

	// 投射物循环音效组件
	UPROPERTY()
	UAudioComponent* ProjectileLoopComponent;

	// 循环音效的衰减设置
	UPROPERTY(EditAnywhere)
	USoundAttenuation* LoopingSoundAttenuation;

	// 火箭弹移动组件
	UPROPERTY(VisibleAnywhere)
	class URocketMovementComponent* RocketMovementComponent;

private:
	// 私有成员（当前未定义）
};
