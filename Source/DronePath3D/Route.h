// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Route.generated.h"

/**
 * 
 */
UCLASS()
class DRONEPATH3D_API URoute : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Route")
	int32 RouteID;
	UPROPERTY(BlueprintReadOnly, Category = "Route")
	int32 PlanID;
	UPROPERTY(BlueprintReadOnly, Category = "Route")
	int32 DroneID;
	UPROPERTY(BlueprintReadOnly, Category = "Route")
	TArray<FVector> Waypoints;
};
