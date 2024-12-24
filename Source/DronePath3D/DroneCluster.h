// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DroneInfo.h"
#include "DroneCluster.generated.h"

/**
 * 
 */
UCLASS()
class DRONEPATH3D_API UDroneCluster : public UObject
{
	GENERATED_BODY()
public:
	TArray<UDroneInfo*> Drones;
	void AddDrone(UDroneInfo* Drone);
	void RemoveDrone(UDroneInfo* Drone);
};
