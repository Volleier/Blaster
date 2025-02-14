// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterHUD.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"

// 在游戏开始时调用
void ABlasterHUD::BeginPlay()
{
	Super::BeginPlay();

	// 添加角色覆盖层
	AddCharacterOverlay();
}

// 添加角色覆盖层
void ABlasterHUD::AddCharacterOverlay()
{
	APlayerController *PlayerController = GetOwningPlayerController();
	if (PlayerController && CharacterOverlayClass)
	{
		// 创建角色覆盖层并添加到视口
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

// 绘制HUD
void ABlasterHUD::DrawHUD()
{
	Super::DrawHUD();
	FVector2D ViewportSize;
	if (GEngine)
	{
		// 获取视口大小
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		// 根据最大扩散值和当前扩散值计算扩散比例
		float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;

		// 绘制中心准星
		if (HUDPackage.CrosshairsCenter)
		{
			FVector2D Spread(0.f, 0.f);
			DrawCrosshairs(HUDPackage.CrosshairsCenter, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		// 绘制左侧准星
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2D Spread(-SpreadScaled, 0.f);
			DrawCrosshairs(HUDPackage.CrosshairsLeft, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		// 绘制右侧准星
		if (HUDPackage.CrosshairsRight)
		{
			FVector2D Spread(SpreadScaled, 0.f);
			DrawCrosshairs(HUDPackage.CrosshairsRight, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		// 绘制顶部准星
		if (HUDPackage.CrosshairsTop)
		{
			FVector2D Spread(0.f, -SpreadScaled);
			DrawCrosshairs(HUDPackage.CrosshairsTop, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		// 绘制底部准星
		if (HUDPackage.CrosshairsBottom)
		{
			FVector2D Spread(0.f, SpreadScaled);
			DrawCrosshairs(HUDPackage.CrosshairsBottom, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
	}
}

// 绘制准星
void ABlasterHUD::DrawCrosshairs(UTexture2D *Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	// 获取纹理宽度和高度
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	// 计算准星绘制位置
	const FVector2D CrosshairDrawPosition(
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y);

	// 绘制纹理
	DrawTexture(
		Texture,
		CrosshairDrawPosition.X,
		CrosshairDrawPosition.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		CrosshairColor);
}
