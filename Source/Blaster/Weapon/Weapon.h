// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

// 枚举类，表示武器的状态
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "初始状态"), // 初始状态
	EWS_Equipped UMETA(DisplayName = "已装备"),	 // 已装备状态
	EWS_Dropped UMETA(DisplayName = "已丢弃"),	 // 已丢弃状态

	EWS_MAX UMETA(DisplayName = "默认最大值") // 默认最大值
};

// 武器类，继承自AActor
UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数
	AWeapon();
	// 每帧调用的Tick函数
	virtual void Tick(float DeltaTime) override;
	// 获取需要复制的属性
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
	// 显示或隐藏拾取小部件
	void ShowPickupWidget(bool bShowWidget);
	void Fire();

protected:
	// 游戏开始时调用的函数
	virtual void BeginPlay() override;

	// 球体组件重叠开始时调用的函数
	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);

	// 球体组件重叠结束时调用的函数
	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex);

private:
	// 武器的骨骼网格组件
	UPROPERTY(VisibleAnywhere, Category = "武器属性")
	USkeletalMeshComponent *WeaponMesh;

	// 武器的球体组件，用于检测重叠
	UPROPERTY(VisibleAnywhere, Category = "武器属性")
	class USphereComponent *AreaSphere;

	// 武器状态，使用OnRep_WeaponState进行复制
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "武器属性")
	EWeaponState WeaponState;

	// 当武器状态发生变化时调用的函数
	UFUNCTION()
	void OnRep_WeaponState();

	// 拾取小部件
	UPROPERTY(VisibleAnywhere, Category = "武器属性")
	class UWidgetComponent *PickupWidget;

	UPROPERTY(EditAnywhere,Category = "武器属性")
	class UAnimationAsset* FireAnimation;

public:
	// 设置武器状态
	void SetWeaponState(EWeaponState State);
	// 获取球体组件
	FORCEINLINE USphereComponent *GetAreaSphere() const { return AreaSphere; }
	// 获取武器的骨骼网格组件
	FORCEINLINE USkeletalMeshComponent *GetWeaponMesh() const { return WeaponMesh; }
};