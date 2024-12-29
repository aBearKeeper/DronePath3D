// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RoutePlan.h"
#include "DroneInfo.h"
#include "TargetPointInfo.h"
#include "UPathPlanningAlgorithm.h"
#include "RouteManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DRONEPATH3D_API URouteManager : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Route Manager")
	static URouteManager* GetSingleton();

	UFUNCTION(BlueprintCallable, Category = "Route Manager")
	void PlanRoutes(TArray<UDroneInfo*> Drones, TArray<UTargetPointInfo*> Points, UUPathPlanningAlgorithm* Algorithm, UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Route Manager")
	void SimulatePlan(URoutePlan* Plan);

	UFUNCTION(BlueprintCallable, Category = "Route Manager")
	void DeletePlan(URoutePlan* Plan);

	UFUNCTION(BlueprintCallable, Category = "Route Manager")
	TArray<URoutePlan*> GetPlansBySceneID(int32 SceneID);

	UPROPERTY(BlueprintReadWrite, Category = "Route Manager")
	TArray<URoutePlan*> Plans;

	UPROPERTY(BlueprintReadWrite, Category = "Route Manager")
	bool TaskAssignmentDroneFirst;

	void Initialize();

private:
	static URouteManager* Singleton;
	
};
URouteManager* URouteManager::Singleton = nullptr;