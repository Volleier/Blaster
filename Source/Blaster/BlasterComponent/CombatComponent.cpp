// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// 战斗组件构造函数
UCombatComponent::UCombatComponent()
{
	// 允许组件每帧更新
	PrimaryComponentTick.bCanEverTick = true;

	BaseWalkSpeed = 600.f; // 设置角色的基础行走速度
	AimWalkSpeed = 450.f;  // 设置角色瞄准时的行走速度
}
// 设置需要在网络中复制的属性
void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon); // 复制装备的武器到所有客户端
	DOREPLIFETIME(UCombatComponent, bAiming);		 // 复制瞄准状态到所有客户端
}

// 组件开始播放时的初始化
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character)
	{
		// 初始化角色的行走速度为基础速度
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
}

// 设置瞄准状态
void UCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming); // 在服务器上同步瞄准状态
	if (Character)
	{
		// 根据瞄准状态动态调整移动速度
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

// 服务器端设置瞄准状态的实现
void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (Character)
	{
		// 在服务器上同步更新移动速度
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

// 当装备武器状态被复制时触发的回调
void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character)
	{
		// 装备武器后更新角色的移动和旋转设置
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}
}

// 组件的Tick函数，每帧更新
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// 处理开火按钮的按下状态
void UCombatComponent::FireButtonPressed(bool bIsPressed)
{
	bFireButtonPressed = bIsPressed;
	if (bFireButtonPressed)
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);   // 进行准星下的射线检测
		ServerFire(HitResult.ImpactPoint); // 在服务器上处理开火事件
	}
}

// 在准星位置进行射线检测
void UCombatComponent::TraceUnderCrosshairs(FHitResult &TraceHitResult)
{
	// 获取视口大小
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// 计算准星在屏幕中的位置
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// 将屏幕坐标转换为世界坐标
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		// 执行射线检测
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;
		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);
	}
}

// 服务器端处理开火的实现
void UCombatComponent::ServerFire_Implementation(const FVector_NetQuantize &TraceHitTarget)
{
	MulticastFire(TraceHitTarget); // 广播开火事件到所有客户端
}

// 多播开火事件的实现（在所有客户端执行）
void UCombatComponent::MulticastFire_Implementation(const FVector_NetQuantize &TraceHitTarget)
{
	if (EquippedWeapon == nullptr)
		return;
	if (Character)
	{
		Character->PlayFireMonatge(bAiming);  // 播放开火动画
		EquippedWeapon->Fire(TraceHitTarget); // 执行武器的开火逻辑
	}
}

// 装备武器
void UCombatComponent::EquipWeapon(AWeapon *WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr)
		return;

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	// 将武器附加到角色的右手插槽
	const USkeletalMeshSocket *HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}

	// 设置武器和角色的相关属性
	EquippedWeapon->SetOwner(Character);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
}