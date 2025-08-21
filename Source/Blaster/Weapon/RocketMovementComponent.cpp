#include "RocketMovementComponent.h"

// 处理火箭移动组件的阻挡碰撞
// 参数：
//   Hit - 碰撞结果信息
//   TimeTick - 当前时间片
//   MoveDelta - 移动的距离向量
//   SubTickTimeRemaining - 剩余的子时间片
// 返回值：
//   EHandleBlockingHitResult 枚举，表示处理结果
URocketMovementComponent::EHandleBlockingHitResult URocketMovementComponent::HandleBlockingHit(
	const FHitResult& Hit,
	float TimeTick,
	const FVector& MoveDelta,
	float& SubTickTimeRemaining)
{
	// 调用父类的阻挡碰撞处理方法
	Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);
	// 火箭继续下一步移动
	return EHandleBlockingHitResult::AdvanceNextSubstep;
}

// 处理火箭的碰撞影响
// 参数：
//   Hit - 碰撞结果信息
//   TimeSlice - 当前时间片
//   MoveDelta - 移动的距离向量
void URocketMovementComponent::HandleImpact(
	const FHitResult& Hit,
	float TimeSlice,
	const FVector& MoveDelta)
{
	// 火箭不应停止移动，仅在其碰撞盒检测到碰撞时爆炸
}