// 填写你的版权声明在项目设置的描述页面。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

// 定义一个HUD包结构体，用于存储HUD相关的纹理和颜色信息
USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	class UTexture2D *CrosshairsCenter; // 中心准星纹理
	UTexture2D *CrosshairsLeft;			// 左侧准星纹理
	UTexture2D *CrosshairsRight;		// 右侧准星纹理
	UTexture2D *CrosshairsTop;			// 顶部准星纹理
	UTexture2D *CrosshairsBottom;		// 底部准星纹理
	float CrosshairSpread;				// 准星扩散值
	FLinearColor CrosshairColor;		// 准星颜色
};

/**
 *
 */
UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()

public:
	// 重写DrawHUD函数，用于绘制HUD
	virtual void DrawHUD() override;

	// 内联函数，用于设置HUD包
	FORCEINLINE void SetHUDPackage(const FHUDPackage &Package) { HUDPackage = Package; }

	// 编辑属性，玩家状态的类别，用于指定角色覆盖类
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	// 角色覆盖指针
	UPROPERTY()
	class UCharacterOverlay *CharacterOverlay; 

	// 添加角色覆盖
	void AddCharacterOverlay();

protected:
	// 重写BeginPlay函数，在游戏开始时调用
	virtual void BeginPlay() override;

private:
	FHUDPackage HUDPackage; // HUD包实例

	// 绘制准星函数
	void DrawCrosshairs(UTexture2D *Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	// 编辑属性，最大准星扩散值
	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;
};
