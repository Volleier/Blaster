// 在项目设置的描述页面填写您的版权声明。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

/**
 * @brief 投射物类（子弹/炮弹等），用于处理投射物的行为和效果
 */
UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief 构造函数
	 */
	AProjectile();

	/**
	 * @brief 每帧调用一次
	 * @param DeltaTime 每帧的时间间隔
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief 销毁时调用
	 */
	virtual void Destroyed() override;

protected:
	/**
	 * @brief 游戏开始时调用
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief 启动销毁计时器
	 */
	void StartDestroyTimer();

	/**
	 * @brief 销毁计时器结束时调用
	 */
	void DestroyTimerFinished();

	/**
	 * @brief 生成轨迹特效
	 */
	void SpawnTrailSystem();

	/**
	 * @brief 造成爆炸伤害
	 */
	void ExplodeDamage();

	/**
	 * @brief 碰撞事件处理函数
	 * @param HitComponent 发生碰撞的组件
	 * @param OtherActor 另一个发生碰撞的Actor
	 * @param OtherComponent 另一个发生碰撞的组件
	 * @param NormalImpulse 碰撞产生的冲量
	 * @param Hit 碰撞结果
	 */
	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		FVector NormalImpulse,
		const FHitResult& Hit);

	/** 子弹伤害数值 */
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	/** 碰撞后生成的粒子特效 */
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	/** 碰撞后播放的音效 */
	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;

	/** 碰撞盒组件 */
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	/** 轨迹特效系统（Niagara） */
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* TrailSystem;

	/** 轨迹特效组件（Niagara） */
	UPROPERTY()
	class UNiagaraComponent* TrailSystemComponent;

	/** 投射物运动组件 */
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	/** 投射物静态网格体组件 */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	/** 爆炸伤害内半径 */
	UPROPERTY(EditAnywhere)
	float DamageInnerRadius = 200.f;

	/** 爆炸伤害外半径 */
	UPROPERTY(EditAnywhere)
	float DamageOuterRadius = 500.f;

private:
	/** 轨迹粒子系统 */
	UPROPERTY(EditAnywhere)
	UParticleSystem* Tracer;

	/** 轨迹粒子系统组件 */
	UPROPERTY()
	class UParticleSystemComponent* TracerComponent;

	/** 销毁计时器句柄 */
	FTimerHandle DestroyTimer;

	/** 自动销毁时间（秒） */
	UPROPERTY(EditAnywhere)
	float DestroyTime = 3.f;

public:
	// 预留扩展
};
