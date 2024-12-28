// Fill out your copyright notice in the Description page of Project Settings.


#include "UPathPlanningAlgorithm.h"

TArray<FVector> UUPathPlanningAlgorithm::ExecuteAlgorithm(FVector StartPoint, FVector TargetPoint)
{
	return TArray<FVector>();
}

void UUPathPlanningAlgorithm::SetParameter(const FString& Key, double Value)
{
	Parameters.Add(Key, Value);
}
