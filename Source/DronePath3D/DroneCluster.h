// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DroneInfo.h"
#include "DroneCluster.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DRONEPATH3D_API UDroneCluster : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Drone Cluster")
	TArray<UDroneInfo*> Drones;
	UPROPERTY(BlueprintReadWrite, Category = "Drone Cluster")
	UDroneInfo* CurrentDrone = nullptr;
	void AddDrone(UDroneInfo* Drone);
	void RemoveDrone(UDroneInfo* Drone);
	UFUNCTION(BlueprintCallable, Category = "Drone Cluster")
	bool setCurrentDroneByName(FString droneName);
};
