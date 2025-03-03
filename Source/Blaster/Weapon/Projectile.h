// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数
	AProjectile();

	// 每帧调用一次
	virtual void Tick(float DeltaTime) override;

	// 销毁时调用
	virtual void Destroyed() override;

protected:
	// 游戏开始时调用
	virtual void BeginPlay() override;

	// 碰撞事件处理函数
	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent *HitComponent,	 // 碰撞的组件
		AActor *OtherActor,					 // 另一个碰撞的Actor
		UPrimitiveComponent *OtherComponent, // 另一个碰撞的组件
		FVector NormalImpulse,				 // 碰撞的冲量
		const FHitResult &Hit				 // 碰撞结果
	);

	// 子弹伤害
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

private:
	// 碰撞盒组件
	UPROPERTY(EditAnywhere)
	class UBoxComponent *CollisionBox;

	// 投射物运动组件
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent *ProjectileMovementComponent;

	// 轨迹粒子系统
	UPROPERTY(EditAnywhere)
	class UParticleSystem *Tracer;

	// 轨迹粒子系统组件
	UPROPERTY()
	class UParticleSystemComponent *TracerComponent;

	// 碰撞时的粒子系统
	UPROPERTY(EditAnywhere)
	UParticleSystem *ImpactParticles;

	// 碰撞时的声音
	UPROPERTY(EditAnywhere)
	class USoundCue *ImpactSound;
};
