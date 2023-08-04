// RogueLike Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RgAIController.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class ROGUELIKEGAME_API ARgAIController : public AAIController
{
	GENERATED_BODY()

protected:

	//��ʾ������ֻ��ͨ�������Դ��ڣ���Ĭ��ֵ���༭,ϸ�ڴ��ڲ��ܱ༭��
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	virtual void BeginPlay() override;
};
