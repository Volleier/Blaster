// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

// 声明一个UCLASS，表示这是一个蓝图可用的类
UCLASS()
class BLASTER_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY() // 生成类的主体
public:
	// 声明一个UTextBlock指针，并使用元数据宏进行绑定
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DisplayText;

	// 设置显示文本的函数声明
	void SetDisplayText(FString TextToDisplay);

	// 声明一个蓝图可调用的函数，用于显示玩家的网络角色
	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* InPawn);

protected:
	// 重写NativeConstruct函数，用于在构造时进行初始化
	virtual void NativeConstruct() override;
};
