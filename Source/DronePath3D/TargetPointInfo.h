// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TargetPointInfo.generated.h"

/**
 * 
 */
UCLASS()
class DRONEPATH3D_API UTargetPointInfo : public UObject
{
	GENERATED_BODY()
public:
	int32 SceneID;
	int32 PointID;
	int32 AssignedDroneID;
	FVector Position;
};
