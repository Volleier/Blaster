#include "ProjectileGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// 构造函数：初始化手雷的网格和运动组件
AProjectileGrenade::AProjectileGrenade()
{
	// 创建并附加静态网格组件（手雷模型）
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	// 初始时关闭碰撞
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 创建并配置投射物运动组件
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	// 运动方向决定旋转
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	// 允许网络同步
	ProjectileMovementComponent->SetIsReplicated(true);
	// 允许弹跳
	ProjectileMovementComponent->bShouldBounce = true;
}

// 游戏开始时调用
void AProjectileGrenade::BeginPlay()
{
	AActor::BeginPlay();

	// 生成拖尾特效
	SpawnTrailSystem();
	// 启动销毁计时器
	StartDestroyTimer();

	// 绑定弹跳事件
	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AProjectileGrenade::OnBounce);
}

// 弹跳事件回调函数
void AProjectileGrenade::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	// 如果设置了弹跳音效，则在当前位置播放
	if (BounceSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			BounceSound,
			GetActorLocation());
	}
}

// 销毁时调用
void AProjectileGrenade::Destroyed()
{
	// 爆炸伤害处理
	ExplodeDamage();
	// 调用父类销毁逻辑
	Super::Destroyed();
}