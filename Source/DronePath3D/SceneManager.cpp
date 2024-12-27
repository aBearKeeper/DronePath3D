// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneManager.h"
#include "DatabaseHelper.h"
#include "FileDialogHelper.h"
#include "DroneManager.h"
// #include "LidarPointCloud.h"


USceneManager* USceneManager::GetSingleton()
{
	if (!Singleton) {
		Singleton = NewObject<USceneManager>();
		Singleton->AddToRoot();
		Singleton->Initialize();
	}
	return Singleton;
}

bool USceneManager::ImportScene(FString FilePath)
{
	if (!UDatabaseHelper::GetSingleton()->AddNewScene(FilePath)) {
		return false;
	}
	Initialize();
	return true;
}

void USceneManager::DeleteScene(int32 ID, FString FilePath)
{
	UDatabaseHelper::GetSingleton()->DeleteScene(ID);
	UFileDialogHelper::RemovePointCloudFile(FilePath);
	Initialize();
}

void USceneManager::DroneRemovalNotice(int32 SceneID, int32 DroneID)
{
	UDatabaseHelper* DatabaseHelper = UDatabaseHelper::GetSingleton();
	for (auto Scene : Scenes) {
		if (Scene->SceneID == SceneID) {
			for (auto point : Scene->TargetPoints) {
				if (point->AssignedDroneID == DroneID) {
					point->AssignedDroneID = 0;
					DatabaseHelper->UpdateTargetPointInfo(point);
				}
			}
			break;
		}
	}
}

void USceneManager::Initialize()
{
	UDatabaseHelper* DatabaseHelper = UDatabaseHelper::GetSingleton();
	Scenes = DatabaseHelper->GetAllScenes();
	UDroneManager::GetSingleton()->Initialize();
	for (auto Scene : Scenes) {
		Scene->TargetPoints = DatabaseHelper->GetSceneTargetPoints(Scene->SceneID);
	}
}
