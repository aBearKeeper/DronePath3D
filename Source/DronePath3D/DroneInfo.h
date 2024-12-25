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
	UPROPERTY(BlueprintReadOnly, Category = "Drone Cluster")
	int32 DroneID;
	UPROPERTY(BlueprintReadOnly, Category = "Drone Cluster")
	int32 SceneID;
	UPROPERTY(BlueprintReadOnly, Category = "Drone Cluster")
	FVector StartPosition;
	UPROPERTY(BlueprintReadOnly, Category = "Drone Cluster")
	float Diameter;
	UPROPERTY(BlueprintReadOnly, Category = "Drone Cluster")
	float Height;
	UPROPERTY(BlueprintReadOnly, Category = "Drone Cluster")
	float MaxHeight;
	UPROPERTY(BlueprintReadOnly, Category = "Drone Cluster")
	float MaxThrust;
	UPROPERTY(BlueprintReadOnly, Category = "Drone Cluster")
	float MaxSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Drone Cluster")
	float Weight;
	UPROPERTY(BlueprintReadOnly, Category = "Drone Cluster")
	float Endurance;
	UPROPERTY(BlueprintReadOnly, Category = "Drone Cluster")
	FString Name;
};
