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
	float Diameter; // TODO
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	float Height; // TODO
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	float MaxHeight;
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	float MaxThrust; // TODO
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	float MaxSpeed; // TODO
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	float Weight; // TODO
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	float Endurance;
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	FString Name;
};
