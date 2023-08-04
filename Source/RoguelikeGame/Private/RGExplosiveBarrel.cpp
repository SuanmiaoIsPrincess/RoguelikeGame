// RogueLike Game, All Rights Reserved.


#include "RGExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ARGExplosiveBarrel::ARGExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// UE�еġ�ģ������ѡ��
	MeshComp->SetSimulatePhysics(true);
	// ��ͬ����UE�н�����ײԤ�衱����Ϊ��PhysicsActor��
	MeshComp->SetCollisionProfileName("PhysicsActor");
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

	ForceComp->Radius = 750.0f;			 // ��ը��Χ
	ForceComp->ImpulseStrength = 700.0f; // �����
	ForceComp->bImpulseVelChange = true; // ����������С����UE��ForceComp�ġ������ٶȱ����

}

void ARGExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	// log��Ϣ��category��log/warning/error�ȱ�ʾ��־����ϸ�̶ȣ���ӡ����������
	UE_LOG(LogTemp, Log, TEXT("OtherActor is %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
	UE_LOG(LogTemp, Warning, TEXT("HHHHHHHHHHHHH"));

	FString CombStr = FString::Printf(TEXT("Hit at %s"), *Hit.ImpactPoint.ToString());
	// ��ȡ���磬λ�ã���ӡ�����ݣ���Ҫattach��actor����ɫ������ʱ�䣬�Ƿ���Ӱ��
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombStr, nullptr, FColor::Green, 2.0f, true);
}

// Called when the game starts or when spawned
void ARGExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->OnComponentHit.AddDynamic(this, &ARGExplosiveBarrel::OnActorHit);
	
}

// Called every frame
void ARGExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

