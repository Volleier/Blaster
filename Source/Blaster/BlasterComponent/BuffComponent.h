// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"

/**
 * UBuffComponent
 * 负责角色的各种Buff效果，包括治疗、护盾补充、速度提升和跳跃能力提升。
 * 可通过蓝图生成，属于自定义组件类。
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLASTER_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** 构造函数 */
	UBuffComponent();

	/** 允许ABlasterCharacter访问私有成员 */
	friend class ABlasterCharacter;

	/**
	 * 治疗Buff
	 * @param HealAmount 治疗总量
	 * @param HealingTime 治疗持续时间
	 */
	void Heal(float HealAmount, float HealingTime);

	/**
	 * 补充护盾Buff
	 * @param ShieldAmount 补充护盾总量
	 * @param ReplenishTime 补充持续时间
	 */
	void ReplenishShield(float ShieldAmount, float ReplenishTime);

	/**
	 * 速度提升Buff
	 * @param BuffBaseSpeed 提升后的基础速度
	 * @param BuffCrouchSpeed 提升后的蹲伏速度
	 * @param BuffTime Buff持续时间
	 */
	void BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime);

	/**
	 * 跳跃能力提升Buff
	 * @param BuffJumpVelocity 提升后的跳跃速度
	 * @param BuffTime Buff持续时间
	 */
	void BuffJump(float BuffJumpVelocity, float BuffTime);

	/**
	 * 设置初始速度
	 * @param BaseSpeed 基础速度
	 * @param CrouchSpeed 蹲伏速度
	 */
	void SetInitialSpeeds(float BaseSpeed, float CrouchSpeed);

	/**
	 * 设置初始跳跃速度
	 * @param Velocity 跳跃速度
	 */
	void SetInitialJumpVelocity(float Velocity);

protected:
	/** 组件初始化时调用 */
	virtual void BeginPlay() override;

	/**
	 * 治疗递增处理
	 * @param DeltaTime 每帧时间
	 */
	void HealRampUp(float DeltaTime);

	/**
	 * 护盾递增处理
	 * @param DeltaTime 每帧时间
	 */
	void ShieldRampUp(float DeltaTime);

private:
	/** 角色指针 */
	UPROPERTY()
	class ABlasterCharacter* Character;

	/** 治疗Buff相关变量 */
	bool bHealing = false;	  // 是否正在治疗
	float HealingRate = 0.f;  // 治疗速率
	float AmountToHeal = 0.f; // 剩余治疗量

	/** 护盾Buff相关变量 */
	bool bReplenishingShield = false;  // 是否正在补充护盾
	float ShieldReplenishRate = 0.f;   // 护盾补充速率
	float ShieldReplenishAmount = 0.f; // 剩余补充护盾量

	/** 速度Buff相关变量 */
	FTimerHandle SpeedBuffTimer; // 速度Buff计时器
	void ResetSpeeds();			 // 重置速度到初始值
	float InitialBaseSpeed;		 // 初始基础速度
	float InitialCrouchSpeed;	 // 初始蹲伏速度

	/**
	 * 多播速度Buff
	 * @param BaseSpeed 新基础速度
	 * @param CrouchSpeed 新蹲伏速度
	 */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpeedBuff(float BaseSpeed, float CrouchSpeed);

	/** 跳跃Buff相关变量 */
	FTimerHandle JumpBuffTimer; // 跳跃Buff计时器
	void ResetJump();			// 重置跳跃速度到初始值
	float InitialJumpVelocity;	// 初始跳跃速度

	/**
	 * 多播跳跃Buff
	 * @param JumpVelocity 新跳跃速度
	 */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastJumpBuff(float JumpVelocity);

public:
	/**
	 * 每帧调用
	 * @param DeltaTime 每帧时间
	 * @param TickType Tick类型
	 * @param ThisTickFunction Tick函数
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};