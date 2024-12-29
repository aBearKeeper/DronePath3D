// Fill out your copyright notice in the Description page of Project Settings.


#include "RouteManager.h"
#include "DatabaseHelper.h"
#include "FileDialogHelper.h"
#include "SceneManager.h"

URouteManager* URouteManager::GetSingleton()
{
	if (!Singleton) {
		Singleton = NewObject<URouteManager>();
		Singleton->AddToRoot();
		Singleton->Initialize();
	}
	return Singleton;
}

void URouteManager::PlanRoutes(TArray<UDroneInfo*> Drones, TArray<UTargetPointInfo*> Points, UUPathPlanningAlgorithm* Algorithm, UObject* WorldContextObject)
{
	TMap<UDroneInfo*, TArray<UTargetPointInfo*>> DroneToPoints;
	for (UDroneInfo* Drone : Drones)
	{
		DroneToPoints.Add(Drone, TArray<UTargetPointInfo*>());
	}
	//分配目标点
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
		// 平均任务量
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
		// 最近距离优先
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
	// 逐一规划路径
	URoutePlan* Plan = NewObject<URoutePlan>();
	USceneManager* SceneManager = USceneManager::GetSingleton();
	Plan->SceneID = SceneManager->CurrentScene->SceneID;
	for (auto Pair : DroneToPoints)
	{
		URoute* Route = NewObject<URoute>();
		Route->DroneID = Pair.Key->DroneID;
		FVector StartPoint = Pair.Key->StartPosition;
		while (Pair.Value.Num() > 0)
		{
			Algorithm->SetParameter("Diameter", Pair.Key->Diameter);
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
			auto result = Algorithm->ExecuteAlgorithm(StartPoint, ClosestPoint->Position, WorldContextObject);
			Route->Waypoints += result;
			StartPoint = ClosestPoint->Position;
			Pair.Value.Remove(ClosestPoint);
		}
		Plan->Routes.Add(Route);
	}
	UDatabaseHelper::GetSingleton()->AddNewRoutePlan(Plan);
	for (auto route : Plan->Routes) {
		UFileDialogHelper::SaveWaypointsToFile(route->RouteID, route->Waypoints);
	}
	Plans.Add(Plan);
}

void URouteManager::SimulatePlan(URoutePlan* Plan)
{

}

void URouteManager::DeletePlan(URoutePlan* Plan)
{
	Plans.Remove(Plan);
	UDatabaseHelper::GetSingleton()->DeleteRoutePlan(Plan->PlanID);
}

TArray<URoutePlan*> URouteManager::GetPlansBySceneID(int32 ScnenID) {
	TArray<URoutePlan*> result;
	for (auto plan : Plans) {
		if (plan->SceneID == ScnenID)
			result.Add(plan);
	}
	return result;
}

void URouteManager::Initialize()
{
	Plans = UDatabaseHelper::GetSingleton()->GetAllRoutePlans();
	for (auto Plan : Plans) {
		for (auto Route : Plan->Routes) {
			UFileDialogHelper::LoadWaypointsFromFile(Route->RouteID, Route->Waypoints);
		}
	}
}
