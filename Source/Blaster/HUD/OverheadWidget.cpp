// Fill out your copyright notice in the Description page of Project Settings.

#include "OverheadWidget.h"
#include "Components/TextBlock.h"

// 设置显示文本
void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

// 显示玩家的网络角色
void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;
	switch (RemoteRole)
	{
	case ENetRole::ROLE_Authority:
		Role = FString("Authority"); // 权限
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString("Autonomous Proxy"); // 自主代理
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString("Simulated Proxy"); // 模拟代理
		break;
	case ENetRole::ROLE_None:
		Role = FString("None"); // 无
		break;
	}
	FString RemoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *Role);
	SetDisplayText(RemoteRoleString);
}

// 本地构造函数
void UOverheadWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DisplayText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DisplayText")));
}
