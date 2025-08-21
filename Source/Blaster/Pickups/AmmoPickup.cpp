#include "AmmoPickup.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/BlasterComponent/CombatComponent.h"

// 当球体碰撞体发生重叠时调用
void AAmmoPickup::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent, // 被重叠的组件
	AActor* OtherActor,						  // 参与重叠的其他Actor
	UPrimitiveComponent* OtherComp,			  // 其他Actor的组件
	int32 OtherBodyIndex,					  // 其他体索引
	bool bFromSweep,						  // 是否为扫掠检测
	const FHitResult& SweepResult			  // 扫掠结果
)
{
	// 调用父类的重叠处理逻辑
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// 尝试将OtherActor转换为ABlasterCharacter类型
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		// 获取角色的Combat组件
		UCombatComponent* Combat = BlasterCharacter->GetCombat();
		if (Combat)
		{
			// 给角色的Combat组件添加弹药
			Combat->PickupAmmo(WeaponType, AmmoAmount);
		}
	}
	// 拾取弹药后销毁自身
	Destroy();
}