// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "BlasterAnimInstance.generated.h"

/**
 * UBlasterAnimInstance
 *
 * 动画实例类，用于处理角色动画相关的变量和逻辑。
 */
UCLASS()
class BLASTER_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 初始化动画实例，在动画蓝图创建时调用
	virtual void NativeInitializeAnimation() override;

	// 每帧更新动画实例，处理动画相关变量
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	// 当前装备的武器实例指针
	class AWeapon* EquippedWeapon;

private:
	// 角色实例指针
	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class ABlasterCharacter* BlasterCharacter;

	// 角色移动速度
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;

	// 是否处于空中（跳跃或下落）
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	// 是否正在加速
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	// 是否装备武器
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bWeaponEquipped;

	// 是否处于蹲下状态
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouched;

	// 是否处于瞄准状态
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	// 偏航偏移量（用于瞄准偏移动画）
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float YawOffset;

	// 倾斜量（角色左右倾斜）
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Lean;

	// 上一帧角色旋转
	FRotator CharacterRotationLastFrame;

	// 当前帧角色旋转
	FRotator CharacterRotation;

	// 旋转差值（用于计算倾斜等动画效果）
	FRotator DeltaRotation;

	// 瞄准偏航角度
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float AO_Yaw;

	// 瞄准俯仰角度
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float AO_Pitch;

	// 左手变换（用于武器握持动画）
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FTransform LeftHandTransform;

	// 转向状态（枚举类型，表示角色转向动画状态）
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	ETurningInPlace TurningInPlace;

	// 右手旋转（用于武器瞄准动画）
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FRotator RightHandRotation;

	// 是否本地控制（用于区分本地玩家与网络玩家）
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bLocallyControlled;

	// 是否旋转根骨骼（用于角色死亡或特殊动画）
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bRotateRootBone;

	// 是否被消除（角色死亡状态）
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bElimmed;

	// 是否启用FABRIK IK（用于手部IK动画）
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bUseFABRIK;

	// 是否启用瞄准偏移动画
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bUseAimOffsets;

	// 是否启用右手变换（用于特殊武器动画）
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bTransformRightHand;
};