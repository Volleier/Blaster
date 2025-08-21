#include "SpeedPickup.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/BlasterComponent/BuffComponent.h"

/*
 * 当球体碰撞体发生重叠时触发的回调函数
 * @param OverlappedComponent 发生重叠的组件
 * @param OtherActor          与本组件重叠的其他Actor
 * @param OtherComp           与本组件重叠的其他组件
 * @param OtherBodyIndex      其他组件的Body索引
 * @param bFromSweep          是否为Sweep造成的重叠
 * @param SweepResult         Sweep的结果信息
 */
void ASpeedPickup::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// 调用父类的重叠处理逻辑
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// 尝试将重叠的Actor转换为ABlasterCharacter类型
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		// 获取角色的Buff组件
		UBuffComponent* Buff = BlasterCharacter->GetBuff();
		if (Buff)
		{
			// 给角色添加速度增益效果
			Buff->BuffSpeed(BaseSpeedBuff, CrouchSpeedBuff, SpeedBuffTime);
		}
	}

	// 拾取后销毁自身
	Destroy();
}