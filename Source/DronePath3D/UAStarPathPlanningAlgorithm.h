// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPathPlanningAlgorithm.h"
#include "UAStarPathPlanningAlgorithm.generated.h"

/**
 * 
 */
UCLASS()
class DRONEPATH3D_API UUAStarPathPlanningAlgorithm : public UUPathPlanningAlgorithm
{
	GENERATED_BODY()
	

private:
	UObject* WorldContextObject;
	int Unit_Distance;

	bool HaveCloudPoints(FVector Position);

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
	UUAStarPathPlanningAlgorithm()
	{
		WorldContextObject = GetWorld();
		Unit_Distance = 100;
	}
	TArray<FVector> ExecuteAlgorithm(FVector StartPoint, FVector TargetPoint) override;

};
