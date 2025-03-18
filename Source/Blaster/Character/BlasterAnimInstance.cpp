// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterAnimInstance.h"
#include "BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blaster/Weapon/Weapon.h"

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 初始化BlasterCharacter
	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	// 如果BlasterCharacter为空，则尝试获取PawnOwner
	if (BlasterCharacter == nullptr)
	{
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	}
	if (BlasterCharacter == nullptr)
		return;

	// 获取角色速度并计算速度大小
	FVector Velocity = BlasterCharacter->GetVelocity();
	Velocity.Z = 0.f;		 // 忽略Z轴速度
	Speed = Velocity.Size(); // 计算速度大小

	// 判断角色是否在空中
	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();
	// 判断角色是否在加速
	bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	// 判断角色是否装备武器
	bWeaponEquipped = BlasterCharacter->IsWeaponEquipped();
	// 获取装备的武器
	EquippedWeapon = BlasterCharacter->GetEquippedWeapon();
	// 判断角色是否蹲下
	bIsCrouched = BlasterCharacter->bIsCrouched;
	// 判断角色是否在瞄准
	bAiming = BlasterCharacter->IsAiming();
	// 获取角色的转向状态
	TurningInPlace = BlasterCharacter->GetTurningInPlace();
	// 获取骨骼的旋转角度
	bRotateRootBone = BlasterCharacter->ShouldRotateRootBone();
	// 获取角色是否被淘汰
	bElimmed = BlasterCharacter->IsElimmed();
	// 计算偏移偏航角用于侧移
	FRotator AimRotation = BlasterCharacter->GetBaseAimRotation();								   // 获取角色的基础瞄准旋转
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity()); // 根据速度向量计算旋转
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation); // 计算旋转差值
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);					   // 插值计算新的旋转差值
	YawOffset = DeltaRotation.Yaw;																   // 设置偏航角度

	// 计算角色的倾斜角度
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
		// 获取左手的变换信息
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		BlasterCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation); // 转换到骨骼空间
		LeftHandTransform.SetLocation(OutPosition);																											  // 设置左手位置
		LeftHandTransform.SetRotation(FQuat(OutRotation));																									  // 设置左手旋转

		FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("Hand_R"), ERelativeTransformSpace::RTS_World); // 获取右手变换信息
		RightHandRotation = UKismetMathLibrary::FindLookAtRotation(
			RightHandTransform.GetLocation(),
			RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - BlasterCharacter->GetHitTarget())); // 计算右手旋转

		if (BlasterCharacter->IsLocallyControlled())
		{
			bLocallyControlled = true;																													   // 设置本地控制标志
			FTransform MuzzleTipTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"), ERelativeTransformSpace::RTS_World); // 获取枪口变换信息
			// 绘制射线
			// DrawDebugLine(
			//	GetWorld(),
			//	MuzzleTipTransform.GetLocation(),
			//	BlasterCharacter->GetHitTarget(),
			//	FColor::Orange
			// );
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
				RightHandTransform.GetLocation(),
				RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - BlasterCharacter->GetHitTarget())); // 计算瞄准旋转
			RightHandRotation = FMath::RInterpTo(
				RightHandRotation,
				LookAtRotation,
				DeltaTime,
				30.f); // 插值计算新的右手旋转
		}
	}
	bUseFABRIK = BlasterCharacter->GetCombatState() != ECombatState::ECS_Reloading;

	bUseAimOffsets = BlasterCharacter->GetCombatState() != ECombatState::ECS_Reloading;
	bTransformRightHand = BlasterCharacter->GetCombatState() != ECombatState::ECS_Reloading;
}