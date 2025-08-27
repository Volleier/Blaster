// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 * UCharacterOverlay
 *
 * 角色界面覆盖层，用于显示角色状态信息（如生命值、护盾、分数等）。
 */
UCLASS()
class BLASTER_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 生命值进度条控件 */
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	/** 生命值文本控件 */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	/** 护盾进度条控件 */
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ShieldBar;

	/** 护盾文本控件 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ShieldText;

	/** 分数文本控件 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreAmount;

	/** 击败数文本控件 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DefeatsAmount;

	/** 武器弹药数量文本控件 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponAmmoAmount;

	/** 携带弹药数量文本控件 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CarriedAmmoAmount;

	/** 比赛倒计时文本控件 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchCountdownText;

	/** 手雷数量文本控件 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GrenadesText;

	/** 高Ping图像 */
	UPROPERTY(meta = (BindWidget))
	class UImage* HighPingImage;

	/** 高Ping图像动画 */
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HighPingAnimation;
};
