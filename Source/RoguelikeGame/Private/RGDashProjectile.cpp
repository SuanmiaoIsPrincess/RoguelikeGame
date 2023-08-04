// RogueLike Game, All Rights Reserved.


#include "RGDashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

void ARGDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DetonateDelay, this, &ARGDashProjectile::Explode, DetonateDelay);
}

void ARGDashProjectile::Explode_Implementation()
{
	//ȷ�������ʱ������Ϊ���ܻ�ͨ������Դ����Explode��OnActorHit��
	GetWorldTimerManager().ClearTimer(TimerHandle_DetonateDelay);

	//��ָ��λ�ò���������
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	//��������ϵͳ
	EffectComp->DeactivateSystem();

	//����ֹͣ�˶�,�ر���ײ�Է�����Hit����Overlap�¼�
	MoveComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	//���õڶ�����ʱ��
	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ARGDashProjectile::TeleportInstigator, TeleportDelay);

}

void ARGDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		//Ŀ��λ�ã�������λ�ã��Ƿ�Ϊһ�����ԣ��Ƿ���������ַ�����Instigator
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
}

ARGDashProjectile::ARGDashProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MoveComp->InitialSpeed = 6000.f;
}
