// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Announcement.generated.h"

/**
 * UAnnouncement
 *
 * 游戏公告界面小部件类，继承自 UUserWidget。
 * 用于在HUD上显示游戏相关的公告信息，包括倒计时、公告文本和附加信息。
 */
UCLASS()
class BLASTER_API UAnnouncement : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * WarmupTime
	 * 绑定到蓝图中的 TextBlock，用于显示游戏准备阶段的倒计时。
	 */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WarmupTime;

	/**
	 * AnnouncementText
	 * 绑定到蓝图中的 TextBlock，用于显示主要公告内容。
	 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AnnouncementText;

	/**
	 * InfoText
	 * 绑定到蓝图中的 TextBlock，用于显示附加信息或提示。
	 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoText;
};