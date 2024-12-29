// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DroneInfo.h"
#include "TargetPointInfo.h"
#include "UPathPlanningAlgorithm.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DRONEPATH3D_API UUPathPlanningAlgorithm : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Path Planning Algorithm")
	FString AlgorithmName;

	UPROPERTY(BlueprintReadOnly, Category = "Path Planning Algorithm")
	TMap<FString, double> Parameters;

public:
	UFUNCTION(BlueprintCallable, Category = "Path Planning Algorithm")
	virtual TArray<FVector> ExecuteAlgorithm(FVector StartPoint, FVector TargetPoint, UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Path Planning Algorithm")
	void SetParameter(const FString& Key, double Value);

	UFUNCTION(BlueprintCallable, Category = "Path Planning Algorithm")
	FString GetAlgorithmName() { return AlgorithmName; }


};
