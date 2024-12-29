// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPathPlanningAlgorithm.h"
#include "PotentialFieldAlgorithm.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DRONEPATH3D_API UPotentialFieldAlgorithm : public UUPathPlanningAlgorithm
{
	GENERATED_BODY()
private:
	bool HaveCloudPoints(FVector Position, UObject* WorldContextObject);

	struct FNode
	{
		FVector Position;//�ڵ�����

		float F_Cost;//��������
		float G_Cost;//Ԥ������
		float Acc_Cost;//���������Ԥ�����۵ĺ�

		FNode* Parent;//���ڵ�

		FNode(FVector CurrentPosition)
			: Position(CurrentPosition), F_Cost(0.0f), G_Cost(0.0f), Acc_Cost(0.0f), Parent(nullptr)
		{
		}

	};

public:
	TArray<FVector> ExecuteAlgorithm(FVector StartPoint, FVector TargetPoint, UObject* WorldContextObject) override;

	UFUNCTION(BlueprintCallable, Category = "Planning Algorithm")
	static UPotentialFieldAlgorithm* getPotentialFieldAlgorithm() {
		return NewObject<UPotentialFieldAlgorithm >();
	}
};
