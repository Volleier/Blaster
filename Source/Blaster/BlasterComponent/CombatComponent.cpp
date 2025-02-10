// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Camera/CameraComponent.h"

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

		if (Character->GetFollowCamera())
		{
			DefaultFOV = Character->GetFollowCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character && Character->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;
		SetHUDCrosshairs(DeltaTime);
		InterFOV(DeltaTime);
	}
}

void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (Character == nullptr || Character->Controller == nullptr) return;
	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
	if (Controller)
	{
		HUD = HUD == nullptr ? Cast<ABlasterHUD>(Controller->GetHUD()) : HUD;
		if (HUD)
		{
			if (EquippedWeapon)
			{
				HUDPackage.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDPackage.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDPackage.CrosshairsRight = EquippedWeapon->CrosshairsRight;
				HUDPackage.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
				HUDPackage.CrosshairsTop = EquippedWeapon->CrosshairsTop;
			}
			else
			{
				HUDPackage.CrosshairsCenter = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
			}
			//计算准星扩散
			// [0,600]->[0,1]
			FVector2D WalkSpeedRange(0.f, Character->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = Character->GetVelocity();
			Velocity.Z = 0.f;

			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(
				WalkSpeedRange,
				VelocityMultiplierRange,
				Velocity.Size()
			);

			if (Character->GetCharacterMovement()->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(
					CrosshairInAirFactor,
					2.25f,
					DeltaTime, 
					2.25f
				);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(
					CrosshairInAirFactor,
					0.f, 
					DeltaTime,
					30.f
				);
			}
			if (bAiming)
			{
				CrosshairAimFactor = FMath::FInterpTo(
					CrosshairAimFactor,
					0.5f,
					DeltaTime,
					30.f
				);
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(
					CrosshairAimFactor,
					0.f,
					DeltaTime,
					30.f
				);
			}

			CrosshairShootingFactor = FMath::FInterpTo(
				CrosshairShootingFactor,
				0.f,
				DeltaTime,
				40.f
			);

			HUDPackage.CrosshairSpread =
				0.5f +
				CrosshairVelocityFactor + 
				CrosshairInAirFactor -
				CrosshairAimFactor +
				CrosshairShootingFactor;

			HUD->SetHUDPackage(HUDPackage);
		}
	}
}

void UCombatComponent::InterFOV(float DeltaTime)
{
	if (EquippedWeapon == nullptr) return;

	if (bAiming)
	{
		CurrentFOV = FMath::FInterpTo(
			CurrentFOV,
			EquippedWeapon->GetZoomedFOV(),
			DeltaTime,
			EquippedWeapon->GetZoomInterSpeed()
		);
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(
			CurrentFOV,
			DefaultFOV,
			DeltaTime,
			ZoomInterSpeed
		);
	}
	if (Character && Character->GetFollowCamera())
	{
		Character->GetFollowCamera()->SetFieldOfView(CurrentFOV);
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

// 处理开火按钮的按下状态
void UCombatComponent::FireButtonPressed(bool bIsPressed)
{
	bFireButtonPressed = bIsPressed;
	if (bFireButtonPressed)
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);   // 进行准星下的射线检测
		ServerFire(HitResult.ImpactPoint); // 在服务器上处理开火事件

		// 设置准星扩散因子
		if (EquippedWeapon)
		{
			CrosshairShootingFactor = 0.8f;
		}
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
		if (TraceHitResult.GetActor() &&
			TraceHitResult.GetActor()->Implements<UInteractWithCrosshairsInterface>())
		{
			HUDPackage.CrosshairColor = FLinearColor::Red;
		}
		else
		{
			HUDPackage.CrosshairColor = FLinearColor::White;
		}
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