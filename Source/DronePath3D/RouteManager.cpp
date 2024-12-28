// Fill out your copyright notice in the Description page of Project Settings.


#include "RouteManager.h"

URouteManager* URouteManager::GetSingleton()
{
	if (!Singleton) {
		Singleton = NewObject<URouteManager>();
		Singleton->AddToRoot();
		Singleton->Initialize();
	}
	return Singleton;
}

void URouteManager::PlanRoutes(TArray<UDroneInfo*> Drones, TArray<UTargetPointInfo*> Points, UUPathPlanningAlgorithm* Algorithm)
{
	TMap<UDroneInfo*, TArray<UTargetPointInfo*>> DroneToPoints;
	for (UDroneInfo* Drone : Drones)
	{
		DroneToPoints.Add(Drone, TArray<UTargetPointInfo*>());
	}
	TArray<UTargetPointInfo*> PointsToRemove;
	for (UTargetPointInfo* Point : Points) {
		if (Point->AssignedDroneID != 0) {
			for (UDroneInfo* Drone : Drones) {
				if (Drone->DroneID == Point->AssignedDroneID) {
					DroneToPoints[Drone].Add(Point);
					PointsToRemove.Add(Point);
					break;
				}
			}
		}
	}
	for (UTargetPointInfo* Point : PointsToRemove) {
		Points.Remove(Point);
	}
	PointsToRemove.Empty();
	if (TaskAssignmentDroneFirst)
	{
		while (Points.Num() > 0)
		{
			for (UDroneInfo* Drone : Drones)
			{
				if (Points.Num() == 0)
				{
					break;
				}
				UTargetPointInfo* ClosestPoint = nullptr;
				float ClosestDistance = 0;
				for (UTargetPointInfo* Point : Points)
				{
					float Distance = FVector::Dist(Drone->StartPosition, Point->Position);
					if (!ClosestPoint || Distance < ClosestDistance)
					{
						ClosestPoint = Point;
						ClosestDistance = Distance;
					}
				}
				DroneToPoints[Drone].Add(ClosestPoint);
				Points.Remove(ClosestPoint);
			}
		}
	}
	else
	{
		for (UTargetPointInfo* Point : Points)
		{
			UDroneInfo* ClosestDrone = nullptr;
			float ClosestDistance = 0;
			for (UDroneInfo* Drone : Drones)
			{
				float Distance = FVector::Dist(Drone->StartPosition, Point->Position);
				if (!ClosestDrone || Distance < ClosestDistance)
				{
					ClosestDrone = Drone;
					ClosestDistance = Distance;
				}
			}
			DroneToPoints[ClosestDrone].Add(Point);
		}
	}
	for (auto Pair : DroneToPoints)
	{
		FVector StartPoint = Pair.Key->StartPosition;
		TArray<FVector> Route;
		while (Pair.Value.Num() > 0)
		{
			UTargetPointInfo* ClosestPoint = nullptr;
			float ClosestDistance = 0;
			for (UTargetPointInfo* Point : Pair.Value)
			{
				float Distance = FVector::Dist(StartPoint, Point->Position);
				if (!ClosestPoint || Distance < ClosestDistance)
				{
					ClosestPoint = Point;
					ClosestDistance = Distance;
				}
			}
			Route += Algorithm->ExecuteAlgorithm(StartPoint, ClosestPoint->Position);
			StartPoint = ClosestPoint->Position;
			Pair.Value.Remove(ClosestPoint);
		}
		// TODO: Create a new RoutePlan object and add it to the Plans array
	}
}

void URouteManager::SimulatePlan(URoutePlan* Plan)
{
}

void URouteManager::DeletePlan(URoutePlan* Plan)
{
}

void URouteManager::Initialize()
{

}
