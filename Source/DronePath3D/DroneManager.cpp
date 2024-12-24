// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneManager.h"
#include "SceneManager.h"
#include "DatabaseHelper.h"

void UDroneManager::ConfigureDroneCluster(USceneInfo* SceneInfo)
{
	UDroneCluster* newCluster = NewObject<UDroneCluster>();
	DroneClusters.Add(SceneInfo, newCluster);
}

void UDroneManager::DeleteDroneCluster(UDroneCluster* DroneCluster)
{
    for (auto It = DroneClusters.CreateIterator(); It; ++It)
    {
        if (It->Value == DroneCluster)
        {
            It.RemoveCurrent();
            break;
        }
    }
}

void UDroneManager::DeleteDroneCluster(USceneInfo* Scene)
{
	DroneClusters.Remove(Scene);
}

UDroneCluster* UDroneManager::GetDroneCluster(USceneInfo* SceneInfo)
{
	return DroneClusters[SceneInfo];
}

UDroneManager* UDroneManager::GetSingleton()
{
	if (!Singleton) {
		Singleton = NewObject<UDroneManager>();
		Singleton->AddToRoot();
		Singleton->Initialize();
	}
	return Singleton;
}

void UDroneManager::Initialize()
{
	USceneManager* SceneManager = USceneManager::GetSingleton();
	for (auto Scene : SceneManager->Scenes)
	{
		ConfigureDroneCluster(Scene);
		UDatabaseHelper* DatabaseHelper = UDatabaseHelper::GetSingleton();
		TArray<UDroneInfo*> Drones = DatabaseHelper->GetAllDrones();
		for (auto Drone : Drones)
		{
			if (Drone->SceneID == Scene->SceneID)
			{
				GetDroneCluster(Scene)->AddDrone(Drone);
			}
		}
	}
}
