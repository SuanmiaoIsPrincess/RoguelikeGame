// RogueLike Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RGGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URGGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGUELIKEGAME_API IRGGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//BlueprintCallable��������ͼ�е���
	//BlueprintImplementableEvent��������ͼ��ʵ��
	//BlueprintNativeEvent����ͼ�ɵ��ÿ�ʵ�֣���Ҫ����д����Ҳ��Ĭ��ʵ��
	// ��������ߡ�Ϊ��ʹ����˫�����ߵĽ�ɫ����ȷ���ã�����ΪPawn������Character
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
};
