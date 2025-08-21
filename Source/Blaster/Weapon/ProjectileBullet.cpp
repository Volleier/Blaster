#include "ProjectileBullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"

// 构造函数：初始化子弹的移动组件
AProjectileBullet::AProjectileBullet()
{
	// 创建并初始化投射物移动组件
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	// 设置投射物的旋转跟随速度方向
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	// 使投射物移动组件支持网络同步
	ProjectileMovementComponent->SetIsReplicated(true);
}

// 当子弹碰撞到其他物体时调用
void AProjectileBullet::OnHit(
	UPrimitiveComponent* HitComp,	// 被击中的组件
	AActor* OtherActor,				// 被击中的Actor
	UPrimitiveComponent* OtherComp, // 被击中的组件
	FVector NormalImpulse,			// 碰撞产生的冲量
	const FHitResult& Hit			// 碰撞结果信息
)
{
	// 获取子弹的拥有者角色
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		// 获取角色的控制器
		AController* OwnerController = OwnerCharacter->Controller;
		if (OwnerController)
		{
			// 对被击中的Actor应用伤害
			UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
		}
	}
	// 调用父类的OnHit方法，确保父类逻辑被执行
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}