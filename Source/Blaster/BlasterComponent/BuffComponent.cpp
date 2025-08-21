// Fill out your copyright notice in the Description page of Project Settings.

#include "BuffComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// 构造函数，设置组件可Tick
UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// 设置角色初始移动速度（站立和蹲下）
void UBuffComponent::SetInitialSpeeds(float BaseSpeed, float CrouchSpeed)
{
	InitialBaseSpeed = BaseSpeed;
	InitialCrouchSpeed = CrouchSpeed;
}

// 设置角色初始跳跃速度
void UBuffComponent::SetInitialJumpVelocity(float Velocity)
{
	InitialJumpVelocity = Velocity;
}

// 增加角色移动速度，持续BuffTime秒
void UBuffComponent::BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime)
{
	if (Character == nullptr)
		return;

	// 设置定时器，BuffTime后重置速度
	Character->GetWorldTimerManager().SetTimer(
		SpeedBuffTimer,
		this,
		&UBuffComponent::ResetSpeeds,
		BuffTime);

	// 修改角色移动速度
	if (Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BuffBaseSpeed;
		Character->GetCharacterMovement()->MaxWalkSpeedCrouched = BuffCrouchSpeed;
	}

	// 多播同步速度Buff
	MulticastSpeedBuff(BuffBaseSpeed, BuffCrouchSpeed);
}

// 重置角色移动速度为初始值
void UBuffComponent::ResetSpeeds()
{
	if (Character == nullptr || Character->GetCharacterMovement() == nullptr)
		return;

	Character->GetCharacterMovement()->MaxWalkSpeed = InitialBaseSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeedCrouched = InitialCrouchSpeed;

	// 多播同步速度重置
	MulticastSpeedBuff(InitialBaseSpeed, InitialCrouchSpeed);
}

// 多播同步角色速度变化
void UBuffComponent::MulticastSpeedBuff_Implementation(float BaseSpeed, float CrouchSpeed)
{
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
		Character->GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	}
}

// 增加角色跳跃速度，持续BuffTime秒
void UBuffComponent::BuffJump(float BuffJumpVelocity, float BuffTime)
{
	if (Character == nullptr)
		return;

	// 设置定时器，BuffTime后重置跳跃速度
	Character->GetWorldTimerManager().SetTimer(
		JumpBuffTimer,
		this,
		&UBuffComponent::ResetJump,
		BuffTime);

	// 修改角色跳跃速度
	if (Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->JumpZVelocity = BuffJumpVelocity;
	}

	// 多播同步跳跃Buff
	MulticastJumpBuff(BuffJumpVelocity);
}

// 多播同步角色跳跃速度变化
void UBuffComponent::MulticastJumpBuff_Implementation(float JumpVelocity)
{
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	}
}

// 重置角色跳跃速度为初始值
void UBuffComponent::ResetJump()
{
	if (Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->JumpZVelocity = InitialJumpVelocity;
	}

	// 多播同步跳跃速度重置
	MulticastJumpBuff(InitialJumpVelocity);
}

// 治疗角色，HealAmount为总治疗量，HealingTime为持续时间
void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
	bHealing = true;
	HealingRate = HealAmount / HealingTime; // 每秒治疗量
	AmountToHeal += HealAmount;				// 累加待治疗量
}

// 补充护盾，ShieldAmount为总补充量，ReplenishTime为持续时间
void UBuffComponent::ReplenishShield(float ShieldAmount, float ReplenishTime)
{
	bReplenishingShield = true;
	ShieldReplenishRate = ShieldAmount / ReplenishTime; // 每秒补充量
	ShieldReplenishAmount += ShieldAmount;				// 累加待补充量
}

// 治疗逐步执行，每帧调用
void UBuffComponent::HealRampUp(float DeltaTime)
{
	if (!bHealing || Character == nullptr || Character->IsElimmed())
		return;

	const float HealThisFrame = HealingRate * DeltaTime; // 本帧治疗量
	Character->SetHealth(FMath::Clamp(Character->GetHealth() + HealThisFrame, 0.f, Character->GetMaxHealth()));
	Character->UpdateHUDHealth();
	AmountToHeal -= HealThisFrame;

	// 治疗结束条件
	if (AmountToHeal <= 0.f || Character->GetHealth() >= Character->GetMaxHealth())
	{
		bHealing = false;
		AmountToHeal = 0.f;
	}
}

// 护盾补充逐步执行，每帧调用
void UBuffComponent::ShieldRampUp(float DeltaTime)
{
	if (!bReplenishingShield || Character == nullptr || Character->IsElimmed())
		return;

	const float ReplenishThisFrame = ShieldReplenishRate * DeltaTime; // 本帧补充量
	Character->SetShield(FMath::Clamp(Character->GetShield() + ReplenishThisFrame, 0.f, Character->GetMaxShield()));
	Character->UpdateHUDShield();
	ShieldReplenishAmount -= ReplenishThisFrame;

	// 补充结束条件
	if (ShieldReplenishAmount <= 0.f || Character->GetShield() >= Character->GetMaxShield())
	{
		bReplenishingShield = false;
		ShieldReplenishAmount = 0.f;
	}
}

// 组件初始化
void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();
}

// 组件Tick，每帧调用
void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 治疗和护盾补充逐步执行
	HealRampUp(DeltaTime);
	ShieldRampUp(DeltaTime);
}