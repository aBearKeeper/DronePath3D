// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SceneInfo.h"
#include "DroneCluster.h"
#include "DroneManager.generated.h"

/**
 * 
 */
UCLASS()
class DRONEPATH3D_API UDroneManager : public UObject
{
	GENERATED_BODY()

public:
	void DeleteDroneCluster(UDroneCluster*);

	void DeleteDroneCluster(USceneInfo*);

	UFUNCTION(BlueprintCallable, Category = "Drone Manager")
	UDroneCluster* GetDroneCluster(USceneInfo* SceneInfo);

	UFUNCTION(BlueprintCallable, Category = "Drone Manager")
	static UDroneManager* GetSingleton();

	UFUNCTION(BlueprintCallable, Category = "Drone Manager")
	bool NewDrone(int32 SceneID, FString Name, FVector Position);
private:
	UPROPERTY()
	TMap<USceneInfo*, UDroneCluster*> DroneClusters;

	void InitDroneCluster(USceneInfo* Scene);

	static UDroneManager* Singleton;

	void Initialize();

	UDroneManager() {}
};

UDroneManager* UDroneManager::Singleton = nullptr;
