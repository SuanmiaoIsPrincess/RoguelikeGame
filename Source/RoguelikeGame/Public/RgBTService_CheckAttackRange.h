// RogueLike Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RgBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEGAME_API URgBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	//��������Ϊ���б�����ʱ����ϸ������б�¶�ؼ�֡ѡ��
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttackRangeKey;

};
