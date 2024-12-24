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
	void ConfigureDroneCluster(USceneInfo* Scene);
	void DeleteDroneCluster(UDroneCluster*);
	void DeleteDroneCluster(USceneInfo*);
	UDroneCluster* GetDroneCluster(USceneInfo* SceneInfo);

	UFUNCTION(BlueprintCallable, Category = "Scene Manager")
	static UDroneManager* GetSingleton();
private:
	TMap<USceneInfo*, UDroneCluster*> DroneClusters;
	static UDroneManager* Singleton;

	void Initialize();

	UDroneManager() {}
};
