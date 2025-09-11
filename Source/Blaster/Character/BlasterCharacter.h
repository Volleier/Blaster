// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "Components/TimelineComponent.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "BlasterCharacter.generated.h"

/**
 * ABlasterCharacter
 * 主要角色类，继承自ACharacter和自定义接口IInteractWithCrosshairsInterface
 * 包含角色的移动、战斗、动画、消除等功能
 */
UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();

	// 每帧调用
	virtual void Tick(float DeltaTime) override;

	// 设置玩家输入组件
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 网络属性复制
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 组件初始化后调用
	virtual void PostInitializeComponents() override;

	// 播放开火动画蒙太奇
	void PlayFireMontage(bool bAiming);

	// 播放换弹动画蒙太奇
	void PlayReloadMontage();

	// 播放消除动画蒙太奇
	void PlayElimMontage();

	// 播放投掷手榴弹动画蒙太奇
	void PlayThrowGrenadeMontage();

	// 多播命中效果
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHit();

	// 复制移动时调用
	virtual void OnRep_ReplicatedMovement() override;

	// 角色消除
	void Elim();

	// 多播消除效果
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();

	// 销毁时调用
	virtual void Destroyed() override;

	// 是否禁用游戏玩法
	UPROPERTY(Replicated)
	bool bDisableGameplay = false;

	// 蓝图实现：显示/隐藏狙击镜UI
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

	// 更新HUD生命值
	void UpdateHUDHealth();

	// 更新HUD护盾
	void UpdateHUDShield();

	// 更新HUD弹药
	void UpdateHUDAmmo();

	// 设置默认武器
	void SpawnDefaultWeapon();

protected:
	// 游戏开始时调用
	virtual void BeginPlay() override;

	// 移动前进
	void MoveForward(float Value);

	// 移动右侧
	void MoveRight(float Value);

	// 转向
	void Turn(float Value);

	// 向上看
	void LookUp(float Value);

	// 装备按钮按下
	void EquipButtonPressed();

	// 蹲下按钮按下
	void CrouchButtonPressed();

	// 瞄准按钮按下
	void AimButtonPressed();

	// 瞄准按钮释放
	void AimButtonReleased();

	// 瞄准偏移
	void AimOffset(float DeltaTime);

	// 跳跃
	virtual void Jump() override;

	// 开火按钮按下
	void FireButtonPressed();

	// 开火按钮释放
	void FireButtonReleased();

	// 换弹按钮按下
	void ReloadButtonPressed();

	// 播放命中反应动画
	void PlayHitReactMontage();

	// 计算瞄准偏移角度
	void CalculateAO_Pitch();

	// 代理转向
	void SimProxiesTurn();

	// 投掷手榴弹按钮按下
	void GrenadeButtonPressed();

	// 丢弃或销毁武器
	void DropOrDestroyWeapon(AWeapon* Weapon);
	void DropOrDestroyWeapons();

	// 角色接受伤害
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		class AController* InstigatorController, AActor* DamageCauser);

	// 轮询相关类并初始化HUD
	void PollInit();

	// 原地旋转
	void RotateInPlace(float DeltaTime);

private:
	// 摄像机臂组件
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	// 跟随摄像机组件
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	// 头顶小部件
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	// 重叠的武器
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	// 重叠武器回调
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	// 战斗组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* Combat;

	// BUFF组件
	UPROPERTY(VisibleAnywhere)
	class UBuffComponent* Buff;

	// 服务器端装备按钮按下
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	// 瞄准偏移的偏航角度
	float AO_Yaw;

	// 插值后的瞄准偏移偏航角度
	float InterpAO_Yaw;

	// 瞄准偏移的俯仰角度
	float AO_Pitch;

	// 初始瞄准旋转
	FRotator StartingAimRotation;

	// 转向状态
	ETurningInPlace TurningInPlace;

	// 原地转向
	void TurnInPlace(float DeltaTime);

	// 开火武器动画蒙太奇
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UAnimMontage* FireWeaponMontage;

	// 换弹动画蒙太奇
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;

	// 命中反应动画蒙太奇
	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* HitReactMontage;

	// 消除动画蒙太奇
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;

	// 投掷手榴弹动画蒙太奇
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ThrowGrenadeMontage;

	// 靠近角色时隐藏摄像机
	void HideCameraIfCharacterClose();

	// 摄像机距离阈值
	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	// 是否旋转根骨骼
	bool bRotateRootBone;

	// 转向阈值
	float TurnThreshold = 0.5f;

	// 代理上次帧的旋转
	FRotator ProxyRotationLastFrame;

	// 代理当前帧的旋转
	FRotator ProxyRotation;

	// 代理的偏航角度
	float ProxyYaw;

	// 距离上次移动复制的时间
	float TimeSinceLastMovementReplication;

	// 计算速度
	float CalculateSpeed();

	// 最大生命值
	UPROPERTY(EditAnywhere, Category = "Player States")
	float MaxHealth = 100.f;

	// 当前生命值
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player States")
	float Health = 100.f;

	// 生命值变化回调
	UFUNCTION()
	void OnRep_Health(float LastHealth);

	// 最大护盾值
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxShield = 100.f;

	// 当前护盾值
	UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, Category = "Player Stats")
	float Shield = 0.f;

	// 护盾变化回调
	UFUNCTION()
	void OnRep_Shield(float LastShield);

	// 角色控制器
	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

	// 是否已消除
	bool bElimmed = false;

	// 消除计时器
	FTimerHandle ElimTimer;

	// 消除延迟
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

	// 消除计时器结束
	void ElimTimerFinished();

	/*
	 * 溶解效果相关
	 */
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;

	// 溶解轨迹
	FOnTimelineFloat DissolveTrack;

	// 溶解曲线
	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;

	// 溶解材质更新
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);

	// 开始溶解
	void StartDissolve();

	// 动态溶解材质实例
	UPROPERTY(VisibleAnywhere, Category = Elim)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// 蓝图设置的溶解材质实例
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* DissolveMaterialInstance;

	/*
	 * 消除特效相关
	 */
	UPROPERTY(EditAnywhere)
	UParticleSystem* ElimBotEffect;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ElimBotComponent;

	UPROPERTY(EditAnywhere)
	class USoundCue* ElimBotSound;

	UPROPERTY()
	class ABlasterPlayerState* BlasterPlayerState;

	/**
	 * 手榴弹相关
	 */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AttachedGrenade;


	/**
	* 默认武器
	*/
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> DefaultWeaponClass;

public:
	// 设置重叠武器
	void SetOverlappingWeapon(AWeapon* Weapon);

	// 是否装备武器
	bool IsWeaponEquipped();

	// 是否在瞄准
	bool IsAiming();

	// 获取瞄准偏移的偏航角度
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }

	// 获取瞄准偏移的俯仰角度
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }

	// 获取装备的武器
	AWeapon* GetEquippedWeapon();

	// 获取转向状态
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }

	// 获取命中目标
	FVector GetHitTarget() const;

	// 获取跟随摄像机
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// 是否应该旋转根骨骼
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }

	// 是否已消除
	FORCEINLINE bool IsElimmed() const { return bElimmed; }

	// 获取生命值
	FORCEINLINE float GetHealth() const { return Health; }

	// 设置生命值
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }

	// 获取最大生命值
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	// 获取护盾值
	FORCEINLINE float GetShield() const { return Shield; }

	// 设置护盾值
	FORCEINLINE void SetShield(float Amount) { Shield = Amount; }

	// 获取最大护盾值
	FORCEINLINE float GetMaxShield() const { return MaxShield; }

	// 获取战斗组件
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }

	// 获取禁用游戏玩法状态
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }

	// 获取换弹动画蒙太奇
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }

	// 获取附加的手榴弹组件
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }

	// 获取战斗状态
	ECombatState GetCombatState() const;

	// 获取BUFF组件
	FORCEINLINE UBuffComponent* GetBuff() const { return Buff; }

	// 设置是否本地装填
	bool IsLocallyReloading();
};
