// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterAnimInstance.h"
#include "BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blaster/Weapon/Weapon.h"

// 动画实例初始化函数
void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 初始化 BlasterCharacter 指针，指向当前拥有者的 Pawn
	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

// 动画实例每帧更新函数
void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	// 如果 BlasterCharacter 为空，则尝试重新获取 PawnOwner
	if (BlasterCharacter == nullptr)
	{
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	}
	if (BlasterCharacter == nullptr)
		return;

	// 获取角色速度并忽略 Z 轴分量
	FVector Velocity = BlasterCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size(); // 计算水平速度大小

	// 判断角色是否在空中
	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();

	// 判断角色是否正在加速
	bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

	// 判断角色是否装备武器
	bWeaponEquipped = BlasterCharacter->IsWeaponEquipped();

	// 获取当前装备的武器
	EquippedWeapon = BlasterCharacter->GetEquippedWeapon();

	// 判断角色是否处于蹲下状态
	bIsCrouched = BlasterCharacter->bIsCrouched;

	// 判断角色是否处于瞄准状态
	bAiming = BlasterCharacter->IsAiming();

	// 获取角色的转向状态
	TurningInPlace = BlasterCharacter->GetTurningInPlace();

	// 获取是否需要旋转根骨骼
	bRotateRootBone = BlasterCharacter->ShouldRotateRootBone();

	// 获取角色是否被淘汰
	bElimmed = BlasterCharacter->IsElimmed();

	// 计算偏移偏航角用于侧移动画
	FRotator AimRotation = BlasterCharacter->GetBaseAimRotation();								   // 获取角色的基础瞄准旋转
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity()); // 根据速度向量计算旋转
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation); // 计算旋转差值
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);					   // 插值计算新的旋转差值
	YawOffset = DeltaRotation.Yaw;																   // 设置偏航角度

	// 计算角色的倾斜角度（Lean）
	CharacterRotationLastFrame = CharacterRotation;																	  // 保存上一帧的角色旋转
	CharacterRotation = BlasterCharacter->GetActorRotation();														  // 获取当前角色旋转
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame); // 计算旋转差值
	const float Target = Delta.Yaw / DeltaTime;																		  // 计算目标倾斜角度
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);											  // 插值计算新的倾斜角度
	Lean = FMath::Clamp(Interp, -90.f, 90.f);																		  // 限制倾斜角度范围

	// 获取角色的瞄准偏航角和俯仰角
	AO_Yaw = BlasterCharacter->GetAO_Yaw();
	AO_Pitch = BlasterCharacter->GetAO_Pitch();

	// 如果装备了武器且武器和角色的网格体存在
	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && BlasterCharacter->GetMesh())
	{
		// 获取左手的世界空间变换信息
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);

		// 将左手位置和旋转转换到角色右手骨骼空间
		FVector OutPosition;
		FRotator OutRotation;
		BlasterCharacter->GetMesh()->TransformToBoneSpace(
			FName("hand_r"),
			LeftHandTransform.GetLocation(),
			FRotator::ZeroRotator,
			OutPosition,
			OutRotation);
		LeftHandTransform.SetLocation(OutPosition);		   // 设置左手位置
		LeftHandTransform.SetRotation(FQuat(OutRotation)); // 设置左手旋转

		// 获取右手的世界空间变换信息
		FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("Hand_R"), ERelativeTransformSpace::RTS_World);

		// 计算右手的旋转，使其朝向目标
		RightHandRotation = UKismetMathLibrary::FindLookAtRotation(
			RightHandTransform.GetLocation(),
			RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - BlasterCharacter->GetHitTarget()));

		// 如果是本地控制的角色，进行右手旋转插值
		if (BlasterCharacter->IsLocallyControlled())
		{
			bLocallyControlled = true; // 设置本地控制标志

			// 获取枪口的世界空间变换信息
			FTransform MuzzleTipTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"), ERelativeTransformSpace::RTS_World);

			// 可选：绘制射线用于调试
			/*
			DrawDebugLine(
				GetWorld(),
				MuzzleTipTransform.GetLocation(),
				BlasterCharacter->GetHitTarget(),
				FColor::Orange
			);
			*/

			// 计算瞄准旋转
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
				RightHandTransform.GetLocation(),
				RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - BlasterCharacter->GetHitTarget()));

			// 插值计算新的右手旋转
			RightHandRotation = FMath::RInterpTo(
				RightHandRotation,
				LookAtRotation,
				DeltaTime,
				30.f);
		}
	}

	// 设置动画相关的布尔变量
	bUseFABRIK = BlasterCharacter->GetCombatState() != ECombatState::ECS_Reloading;
	bUseFABRIK = BlasterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied;
	if (BlasterCharacter->IsLocallyControlled() && BlasterCharacter->GetCombatState() != ECombatState::ECS_ThrowingGrenade)
	{
		bUseFABRIK = !BlasterCharacter->IsLocallyReloading();
	}
	bUseAimOffsets = BlasterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied && !BlasterCharacter->GetDisableGameplay();
	bTransformRightHand = BlasterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied && !BlasterCharacter->GetDisableGameplay();
}