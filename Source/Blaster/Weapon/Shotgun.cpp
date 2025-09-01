#include "Shotgun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"

// Shotgun类的Fire函数实现
// 负责霰弹枪的射击逻辑，包括弹道检测、伤害统计、特效和音效播放
void AShotgun::Fire(const FVector& HitTarget)
{
	// 调用父类武器的Fire方法
	AWeapon::Fire(HitTarget);

	// 获取武器拥有者的Pawn对象
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return;

	// 获取拥有者的控制器
	AController* InstigatorController = OwnerPawn->GetController();

	// 获取武器模型上的“MuzzleFlash”插槽
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		// 获取插槽的变换信息
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		// 获取插槽的位置作为射击起点
		FVector Start = SocketTransform.GetLocation();

		// 用于统计每个角色被击中的弹丸数量
		TMap<ABlasterCharacter*, uint32> HitMap;

		// 循环发射所有弹丸
		for (uint32 i = 0; i < NumberOfPellets; i++)
		{
			FHitResult FireHit;
			// 进行弹道检测，获取击中结果
			WeaponTraceHit(Start, HitTarget, FireHit);

			// 判断是否击中BlasterCharacter角色
			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FireHit.GetActor());
			if (BlasterCharacter && HasAuthority() && InstigatorController)
			{
				// 统计该角色被击中的弹丸数量
				if (HitMap.Contains(BlasterCharacter))
				{
					HitMap[BlasterCharacter]++;
				}
				else
				{
					HitMap.Emplace(BlasterCharacter, 1);
				}
			}

			// 播放击中粒子特效
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticles,
					FireHit.ImpactPoint,
					FireHit.ImpactNormal.Rotation());
			}

			// 播放击中音效
			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					this,
					HitSound,
					FireHit.ImpactPoint,
					.5f,
					FMath::FRandRange(-.5f, .5f));
			}
		}

		// 对每个被击中的角色应用伤害
		for (auto HitPair : HitMap)
		{
			if (HitPair.Key && HasAuthority() && InstigatorController)
			{
				UGameplayStatics::ApplyDamage(
					HitPair.Key,
					Damage * HitPair.Value, // 总伤害 = 单颗弹丸伤害 * 命中次数
					InstigatorController,
					this,
					UDamageType::StaticClass());
			}
		}
	}
}

// 计算霰弹枪弹丸的散射终点位置
void AShotgun::ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector>& HitTargets)
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket == nullptr) return;

	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	const FVector TraceStart = SocketTransform.GetLocation();

	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;

	for (uint32 i = 0; i < NumberOfPellets; i++)
	{
		const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
		const FVector EndLoc = SphereCenter + RandVec;
		FVector ToEndLoc = EndLoc - TraceStart;
		ToEndLoc = TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size();

		HitTargets.Add(ToEndLoc);
	}
}