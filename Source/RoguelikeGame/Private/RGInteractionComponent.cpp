// RogueLike Game, All Rights Reserved.


#include "RGInteractionComponent.h"
#include "RGGameplayInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
URGInteractionComponent::URGInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URGInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URGInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URGInteractionComponent::PrimaryInteract()
{

	TArray<FHitResult> Hits; // �����

	float Radius = 30.f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	FVector EyeLocation; // ��ɫ�۾�λ��
	FRotator EyeRotation; // ��ɫ���߷���
	AActor* MyOwner = GetOwner(); // ��ȡ���ƽ�ɫ
	// ��������ߵ�λ�úͷ��������EyeLocation��EyeRotation
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	// �������߷���ģ�͵��۾�λ�ÿ�ʼ1000cm����ĵ�Ϊ�յ�
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	FCollisionObjectQueryParams ObjectQueryParams; // ��ѯ����
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic); // ѡ���ѯ������̬����

	//GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits)
	{	// ���жϽ���л�ȡ��⵽��Actor��û��⵽��Ϊ��
		AActor* HitActor = Hit.GetActor();
		if (HitActor) {
			// �����⵽actor��Ϊ�գ����ж�actor��û��ʵ��SurGameplayInterface��
			if (HitActor->Implements<URGGameplayInterface>()) {
				// ���Ƕ����Interact()����ΪPawn���ͣ����������ת��
				APawn* MyPawn = Cast<APawn>(MyOwner);
				// ��̬�����ݴ����HitActor������Ӧ����
				// ��һ����������Ϊ�գ���������Ѿ��пգ��ڶ����������Զ���ģ���ʱû��Ӱ�죬���Բ��п�
				IRGGameplayInterface::Execute_Interact(HitActor, MyPawn);
				// ����debug������������ײ�����ߣ���ɫ
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
		break;
	}
	DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Red, false, 3);

}