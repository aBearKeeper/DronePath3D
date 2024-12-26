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

void USceneManager::Initialize()
{
	Scenes = UDatabaseHelper::GetSingleton()->GetAllScenes();
	UDroneManager::GetSingleton()->Initialize();
}
