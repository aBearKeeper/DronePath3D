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
