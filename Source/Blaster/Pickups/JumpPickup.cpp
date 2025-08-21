#include "JumpPickup.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/BlasterComponent/BuffComponent.h"

// 当球体碰撞体发生重叠时调用此函数
void AJumpPickup::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent, // 被重叠的组件
	AActor* OtherActor,                       // 参与重叠的其他Actor
	UPrimitiveComponent* OtherComp,           // 其他Actor的组件
	int32 OtherBodyIndex,                     // 其他体的索引
	bool bFromSweep,                          // 是否为扫掠检测
	const FHitResult& SweepResult             // 扫掠检测结果
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
			// 对角色施加跳跃增益效果
			Buff->BuffJump(JumpZVelocityBuff, JumpBuffTime);
		}
	}

	// 销毁拾取物自身
	Destroy();
}