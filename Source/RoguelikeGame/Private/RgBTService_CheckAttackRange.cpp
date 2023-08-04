// RogueLike Game, All Rights Reserved.


#include "RgBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void URgBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//�����õ��ڰ����
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		//��ȡ�ڰ�������Լ������TargetActor������Ҫ���ڰ��е�������ͬ
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			//��ȡAI
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					//�õ�TargetActor��AIPawn�ľ���
					float Distance = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					
					bool bWithinRange = Distance < 1500.f;
					//������������
					bool bHasLOS = false;
					if (bWithinRange)
					{
						bHasLOS = MyController->LineOfSightTo(TargetActor);
					}

					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLOS);
				}
			}
		}
	}
}
