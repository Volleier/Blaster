#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include "Weapon.generated.h"

/*
 * 枚举类：表示武器的状态
 */
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "初始状态"),      // 初始状态
	EWS_Equipped UMETA(DisplayName = "已装备"),      // 已装备状态
	EWS_Dropped UMETA(DisplayName = "已丢弃"),       // 已丢弃状态
	EWS_MAX UMETA(DisplayName = "默认最大值")        // 默认最大值
};

/*
 * 武器类，继承自AActor
 */
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 当拥有者发生变化时调用
	virtual void OnRep_Owner() override;

	// 设置HUD中的弹药显示
	void SetHUDAmmo();

	// 显示或隐藏拾取小部件
	void ShowPickupWidget(bool bShowWidget);

	// 武器开火逻辑
	virtual void Fire(const FVector& HitTarget);

	// 武器丢弃逻辑
	void Dropped();

	// 增加弹药
	void AddAmmo(int32 AmmoToAdd);

	/*
	 * 武器准星材质
	 */
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	class UTexture2D* CrosshairsCenter;    // 中心准星

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsLeft;            // 左侧准星

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsRight;           // 右侧准星

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsTop;             // 上方准星

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsBottom;          // 下方准星

	/*
	 * 瞄准时放大视野的FOV
	 */
	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;                // 放大后的FOV

	UPROPERTY(EditAnywhere)
	float ZoomInterSpeed = 20.f;           // FOV插值速度

	/*
	 * 自动开火相关
	 */
	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .15f;                // 自动开火延迟

	UPROPERTY(EditAnywhere, Category = Combat)
	bool bAutomatic = true;                // 是否自动开火

	// 装备武器时的音效
	UPROPERTY(EditAnywhere)
	class USoundCue* EquipSound;

	/*
	 * 允许或禁止自定义深度（用于描边等效果）
	 */
	void EnableCustomDepth(bool bEnable);
	
	// 是否销毁武器
	bool bDestroyWeapon = false;

protected:
	// 游戏开始时调用
	virtual void BeginPlay() override;

	// 球体组件重叠开始时调用
	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// 球体组件重叠结束时调用
	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	// 武器的骨骼网格组件
	UPROPERTY(VisibleAnywhere, Category = "武器属性")
	USkeletalMeshComponent* WeaponMesh;

	// 武器的球体组件，用于检测拾取范围
	UPROPERTY(VisibleAnywhere, Category = "武器属性")
	class USphereComponent* AreaSphere;

	// 武器状态，使用OnRep_WeaponState进行网络同步
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "武器属性")
	EWeaponState WeaponState;

	// 当武器状态发生变化时调用
	UFUNCTION()
	void OnRep_WeaponState();

	// 拾取小部件（用于显示拾取提示）
	UPROPERTY(VisibleAnywhere, Category = "武器属性")
	class UWidgetComponent* PickupWidget;

	// 武器开火动画
	UPROPERTY(EditAnywhere, Category = "武器属性")
	class UAnimationAsset* FireAnimation;

	// 弹壳类（用于生成弹壳）
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;

	// 当前弹药数量，使用OnRep_Ammo进行网络同步
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Ammo)
	int32 Ammo;

	// 当弹药数量发生变化时调用
	UFUNCTION()
	void OnRep_Ammo();

	// 消耗一发子弹
	void SpendRound();

	// 弹匣容量
	UPROPERTY(EditAnywhere)
	int32 MagCapacity;

	// 武器拥有者角色指针
	UPROPERTY()
	class ABlasterCharacter* BlasterOwnerCharacter;

	// 武器拥有者控制器指针
	UPROPERTY()
	class ABlasterPlayerController* BlasterOwnerController;

	// 武器类型
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

public:
	// 设置武器状态
	void SetWeaponState(EWeaponState State);

	// 获取球体组件
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }

	// 获取武器骨骼网格组件
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	// 获取瞄准FOV
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }

	// 获取FOV插值速度
	FORCEINLINE float GetZoomInterSpeed() const { return ZoomInterSpeed; }

	// 判断弹药是否为空
	bool IsEmpty();

	// 判断弹药是否已满
	bool IsFull();

	// 获取武器类型
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

	// 获取当前弹药数量
	FORCEINLINE int32 GetAmmo() const { return Ammo; }

	// 获取弹匣容量
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; }
};