	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Components/ActorComponent.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "CombatComponent.generated.h"

// 定义射线检测长度
#define TRACE_LENGTH 50000.f

/**
 * 战斗组件，处理角色的武器装备、射击、装填、投掷等战斗相关逻辑
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// 构造函数
	UCombatComponent();

	// 允许ABlasterCharacter访问私有成员
	friend class ABlasterCharacter;

	// 每帧更新
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 网络同步属性
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 切换武器
	void SwapWeapons();

	// 装备武器
	void EquipWeapon(class AWeapon* WeaponToEquip);

	// 装填弹药
	void Reload();

	// 结束装填（蓝图可调用）
	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	// 处理开火按钮按下事件
	void FireButtonPressed(bool bIsPressed);

	// 霰弹枪装填一发（蓝图可调用）
	UFUNCTION(BlueprintCallable)
	void ShotgunShellReload();

	// 跳转到霰弹枪装填结束
	void JumpToShotgunEnd();

	// 投掷手雷结束（蓝图可调用）
	UFUNCTION(BlueprintCallable)
	void ThrowGrenadeFinished();

	// 发射手雷（蓝图可调用）
	UFUNCTION(BlueprintCallable)
	void LaunchGrenade();

	// 服务器端发射手雷
	UFUNCTION(Server, Reliable)
	void ServerLaunchGrenade(const FVector_NetQuantize& Target);

	// 捡起弹药
	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);

	// 是否本地装填
	bool bLocallyReloading = false;

protected:
	// 初始化
	virtual void BeginPlay() override;

	// 设置瞄准状态
	void SetAiming(bool bIsAiming);

	// 服务器端设置瞄准状态
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	// 装备武器同步回调
	UFUNCTION()
	void OnRep_EquippedWeapon();

	// 副武器同步回调
	UFUNCTION()
	void OnRep_SecondaryWeapon();

	// 执行开火操作
	void Fire();

	/*
	*	根据武器类型执行不同的开火逻辑
	*/
	// 发射弹药武器
	void FireProjectileWeapon();
	//	发射命中武器
	void FireHitScanWeapon();
	// 发射霰弹枪
	void FireShotgun();

	// 本地处理开火请求
	void LocalFire(const FVector_NetQuantize& TraceHitTarget);

	// 本地处理霰弹枪开火请求
	void ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	// 服务器端处理开火请求
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	// 多播开火事件
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	// 服务器端处理霰弹枪开火请求
	UFUNCTION(Server, Reliable)
	void ServerShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	// 多播霰弹枪开火事件
	UFUNCTION(NetMulticast, Reliable)
	void MulticastShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	// 在准星下进行射线检测
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	// 设置HUD准星
	void SetHUDCrosshairs(float DeltaTime);

	// 服务器端装填弹药
	UFUNCTION(Server, Reliable)
	void ServerReload();

	// 处理装填逻辑
	void HandleReload();

	// 计算可装填弹药数量
	int32 AmountToReload();

	// 投掷手雷
	void ThrowGrenade();

	// 服务器端投掷手雷
	UFUNCTION(Server, Reliable)
	void ServerThrowGrenade();

	// 手雷类
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> GrenadeClass;

	// 丢弃装备武器
	void DropEquippedWeapon();

	// 附加Actor到右手
	void AttachActorToRightHand(AActor* ActorToAttach);

	// 附加Actor到左手
	void AttachActorToLeftHand(AActor* ActorToAttach);

	// 附加Actor到背包
	void AttachActorToBackpack(AActor* ActorToAttach);

	// 更新携带弹药
	void UpdateCarriedAmmo();

	// 播放装备武器音效
	void PlayEquipWeaponSound(AWeapon* WeaponToEquip);

	// 播放装备武器音效
	void PlayEquipWeaponSound();

	// 装填空武器
	void ReloadEmptyWeapon();

	// 显示/隐藏已附加的手雷
	void ShowAttachedGrenade(bool bShowGrenade);

	// 更新霰弹枪弹药数值
	void UpdateShotgunAmmoValues();

	// 装备主武器
	void EquipPrimaryWeapon(AWeapon* WeaponToEquip);
	
	// 装备副武器
	void EquipSecondaryWeapon(AWeapon* WeaponToEquip);

	// 当前手雷数量（同步，带回调）
	UPROPERTY(ReplicatedUsing = OnRep_Grenades)
	int32 Grenades = 4;

	// 手雷数量同步回调
	UFUNCTION()
	void OnRep_Grenades();

	// 最大手雷数量
	UPROPERTY(EditAnywhere)
	int32 MaxGrenades = 4;

	// 更新HUD手雷显示
	void UpdateHUDGrenades();

private:
	// 角色指针
	UPROPERTY()
	class ABlasterCharacter* Character;

	// 控制器指针
	UPROPERTY()
	class ABlasterPlayerController* Controller;

	// HUD指针
	UPROPERTY()
	class ABlasterHUD* HUD;

	// 当前装备的武器（同步，带回调）
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	// 副武器（同步，带回调）
	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
	AWeapon* SecondaryWeapon;

	// 是否处于瞄准状态（同步）
	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
	bool bAiming = false;

	// 是否点击瞄准键
	bool bAimButtonPressed = false;

	// 瞄准状态同步回调
	UFUNCTION()
	void OnRep_Aiming();

	// 基础行走速度
	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	// 瞄准时行走速度
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	// 是否按下开火键
	bool bFireButtonPressed;

	// HUD准星相关因子
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	// 击中目标的位置
	FVector HitTarget;

	// HUD包，用于设置准星
	FHUDPackage HUDPackage;

	/*
	 * 瞄准与视野相关
	 */
	 // 默认视野（未瞄准），在BeginPlay中初始化
	float DefaultFOV;

	// 瞄准时视野
	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV = 30.f;

	// 当前视野
	float CurrentFOV;

	// 瞄准时视野插值速度
	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterSpeed = 20.f;

	// 插值更新视野
	void InterFOV(float DeltaTime);

	/*
	 * 自动开火相关
	 */
	 // 开火计时器句柄
	FTimerHandle FireTimer;

	// 是否可以开火
	bool bCanFire = true;

	// 开始开火计时器
	void StartFireTimer();

	// 开火计时器结束
	void FireTimerFinished();

	// 是否可以开火
	bool CanFire();

	// 当前携带弹药（同步，带回调）
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	// 携带弹药同步回调
	UFUNCTION()
	void OnRep_CarriedAmmo();

	// 各武器类型携带弹药映射
	TMap<EWeaponType, int32> CarriedAmmoMap;

	// 最大携带弹药
	UPROPERTY(EditAnywhere)
	int32 MaxCarriedAmmo = 500;

	// 初始弹药
	UPROPERTY(EditAnywhere)
	int StartingAmmo = 30;

	// 各武器类型初始弹药
	UPROPERTY(EditAnywhere)
	int32 StartingRocketAmmo = 0;

	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmo = 0;

	UPROPERTY(EditAnywhere)
	int32 StartingSMGlAmmo = 0;

	UPROPERTY(EditAnywhere)
	int32 StartingShotgunAmmo = 0;

	UPROPERTY(EditAnywhere)
	int32 StartingSniperRifleAmmo = 0;

	UPROPERTY(EditAnywhere)
	int32 StartingGrenadeLauncherAmmo = 0;

	// 初始化携带弹药
	void InitializeCarriedAmmo();

	// 当前战斗状态（同步，带回调）
	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	// 战斗状态同步回调
	UFUNCTION()
	void OnRep_CombatState();

	// 更新弹药数值
	void UpdateAmmoValues();

public:
	// 获取当前手雷数量
	FORCEINLINE int32 GetGrenades() const { return Grenades; }

	// 是否切换武器
	bool ShouldSwapWeapons();
};