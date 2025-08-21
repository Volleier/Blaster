#include "HealthPickup.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/BlasterComponent/BuffComponent.h"

// 构造函数，初始化拾取物
AHealthPickup::AHealthPickup()
{
	// 设置该Actor支持网络同步
	bReplicates = true;
}

// 当球体碰撞体发生重叠时调用
void AHealthPickup::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent, // 被重叠的组件
	AActor* OtherActor,						  // 参与重叠的其他Actor
	UPrimitiveComponent* OtherComp,			  // 其他Actor的组件
	int32 OtherBodyIndex,					  // 其他体索引
	bool bFromSweep,						  // 是否为扫掠检测
	const FHitResult& SweepResult			  // 碰撞结果
)
{
	// 调用父类的重叠处理逻辑
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// 尝试将重叠的Actor转换为Blaster角色
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		// 获取角色的Buff组件
		UBuffComponent* Buff = BlasterCharacter->GetBuff();
		if (Buff)
		{
			// 调用Buff组件的治疗方法，传入治疗量和治疗时间
			Buff->Heal(HealAmount, HealingTime);
		}
	}

	// 拾取后销毁自身
	Destroy();
}