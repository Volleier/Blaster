#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

UCLASS()
class BLASTER_API ACasing : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数
	ACasing();

protected:
	// 游戏开始时调用
	virtual void BeginPlay() override;

	// 当弹壳碰撞时调用的函数
	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComponent,	 // 碰撞组件
		AActor* OtherActor,					 // 其他碰撞的演员
		UPrimitiveComponent* OtherComponent, // 其他碰撞的组件
		FVector NormalImpulse,				 // 碰撞的冲击力
		const FHitResult& Hit				 // 碰撞结果
	);

private:
	// 弹壳的静态网格组件
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CasingMesh;

	// 弹壳弹出时的冲击力
	UPROPERTY(EditAnywhere)
	float ShellEjectionImpulse;

	// 弹壳声音
	UPROPERTY(EditAnywhere)
	class USoundCue* ShellSound;
};
