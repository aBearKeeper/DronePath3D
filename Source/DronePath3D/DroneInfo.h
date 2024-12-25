// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DroneInfo.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DRONEPATH3D_API UDroneInfo : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	int32 DroneID;
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	int32 SceneID;
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	FVector StartPosition;
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	float Diameter;
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	float Height;
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	float MaxHeight;
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	float MaxThrust;
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	float MaxSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	float Weight;
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	float Endurance;
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	FString Name;
};
