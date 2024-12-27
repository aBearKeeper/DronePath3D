// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneCluster.h"

void UDroneCluster::AddDrone(UDroneInfo* Drone)
{
	Drones.Add(Drone);
}

void UDroneCluster::RemoveDrone(UDroneInfo* Drone)
{
	Drones.Remove(Drone);
}

bool UDroneCluster::setCurrentDroneByName(FString droneName)
{
	for (auto Drone : Drones) {
		if (Drone->Name == droneName) {
			CurrentDrone = Drone;
			return true;
		}
	}
	return false;
}

UDroneInfo* UDroneCluster::getDroneByName(FString droneName)
{
	for (auto drone : Drones) {
		if (drone->Name == droneName) {
			return drone;
		}
	}
	return nullptr;
}

UDroneInfo* UDroneCluster::getDroneByID(int32 droneID)
{
	for (auto drone : Drones) {
		if (drone->DroneID == droneID) {
			return drone;
		}
	}
	return nullptr;
}
