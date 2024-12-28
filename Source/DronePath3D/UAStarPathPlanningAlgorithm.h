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
		FVector Position;//节点坐标

		float F_Cost;//基础距离
		float G_Cost;//预估代价
		float Acc_Cost;//基础距离和预估代价的和

		FNode* Parent;//父节点

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
