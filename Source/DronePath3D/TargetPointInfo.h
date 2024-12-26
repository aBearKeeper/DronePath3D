// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TargetPointInfo.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DRONEPATH3D_API UTargetPointInfo : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Target Point Info")
	int32 PointID;
	int32 SceneID;
	UPROPERTY(BlueprintReadWrite, Category = "Target Point Info")
	int32 AssignedDroneID;
	UPROPERTY(BlueprintReadWrite, Category = "Target Point Info")
	FVector Position;
};
