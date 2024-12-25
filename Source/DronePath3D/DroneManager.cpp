// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneManager.h"
#include "SceneManager.h"
#include "DatabaseHelper.h"

void UDroneManager::InitDroneCluster(USceneInfo* SceneInfo)
{
	UDroneCluster* newCluster = NewObject<UDroneCluster>();
	DroneClusters.FindOrAdd(SceneInfo) = newCluster;
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

bool UDroneManager::NewDrone(int32 SceneID, FString Name, FVector Position)
{
	UDroneInfo* DroneInfo = NewObject<UDroneInfo>();
	DroneInfo->SceneID = SceneID;
	DroneInfo->Name = Name;
	DroneInfo->StartPosition = Position;
	if (UDatabaseHelper::GetSingleton()->AddNewDrone(DroneInfo))
	{
		Initialize();		
		return true;
	}
	return false;
}

void UDroneManager::Initialize()
{
	USceneManager* SceneManager = USceneManager::GetSingleton();
	UDatabaseHelper* DatabaseHelper = UDatabaseHelper::GetSingleton();
	TArray<UDroneInfo*> Drones = DatabaseHelper->GetAllDrones();
	for (auto Scene : SceneManager->Scenes)
	{
		InitDroneCluster(Scene);
		for (auto Drone : Drones)
		{
			if (Drone->SceneID == Scene->SceneID)
			{
				GetDroneCluster(Scene)->AddDrone(Drone);
			}
		}
	}
}
