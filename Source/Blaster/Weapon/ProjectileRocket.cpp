#include "ProjectileRocket.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraSystemInstance.h"
#include "Sound/SoundCue.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "RocketMovementComponent.h"
#include "NiagaraFunctionLibrary.h"

// 构造函数：初始化火箭弹的各个组件
AProjectileRocket::AProjectileRocket()
{
	// 创建火箭网格组件，并附加到根组件
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	// 初始时关闭碰撞
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 创建火箭移动组件，并设置属性
	RocketMovementComponent = CreateDefaultSubobject<URocketMovementComponent>(TEXT("RocketMovementComponent"));
	RocketMovementComponent->bRotationFollowsVelocity = true; // 旋转跟随速度方向
	RocketMovementComponent->SetIsReplicated(true);			  // 支持网络同步
}

// 游戏开始时的初始化逻辑
void AProjectileRocket::BeginPlay()
{
	Super::BeginPlay();

	// 非服务器时，绑定碰撞事件
	if (!HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectileRocket::OnHit);
	}

	// 生成火箭拖尾特效
	SpawnTrailSystem();

	// 如果有循环音效和衰减设置，则附加循环音效
	if (ProjectileLoop && LoopingSoundAttenuation)
	{
		ProjectileLoopComponent = UGameplayStatics::SpawnSoundAttached(
			ProjectileLoop,						// 循环音效
			GetRootComponent(),					// 附加到根组件
			FName(),							// 无需指定插槽名
			GetActorLocation(),					// 位置
			EAttachLocation::KeepWorldPosition, // 保持世界位置
			false,								// 不停止已存在的音效
			1.f,								// 音量
			1.f,								// 音调
			0.f,								// 开始时间
			LoopingSoundAttenuation,			// 衰减设置
			(USoundConcurrency*)nullptr,		// 并发设置
			false								// 不自动销毁
		);
	}
}

// 碰撞事件处理函数
void AProjectileRocket::OnHit(
	UPrimitiveComponent* HitComp,	// 被碰撞的组件
	AActor* OtherActor,				// 其他碰撞的Actor
	UPrimitiveComponent* OtherComp, // 其他碰撞的组件
	FVector NormalImpulse,			// 碰撞产生的冲量
	const FHitResult& Hit			// 碰撞结果
)
{
	StartDestroyTimer(); // 启动销毁计时器

	// 生成爆炸粒子特效
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	// 播放爆炸音效
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
	// 隐藏火箭网格
	if (ProjectileMesh)
	{
		ProjectileMesh->SetVisibility(false);
	}
	// 关闭碰撞盒的碰撞
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	// 停止拖尾特效
	if (TrailSystemComponent)
	{
		TrailSystemComponent->GetSystemInstance()->Deactivate();
	}
	// 停止循环音效
	if (ProjectileLoopComponent && ProjectileLoopComponent->IsPlaying())
	{
		ProjectileLoopComponent->Stop();
	}
	// 如果碰撞的是自身拥有者，则不处理伤害
	if (OtherActor == GetOwner())
	{
		return;
	}

	// 造成爆炸伤害
	ExplodeDamage();
}

// 销毁时的处理逻辑（可扩展）
void AProjectileRocket::Destroyed()
{
}