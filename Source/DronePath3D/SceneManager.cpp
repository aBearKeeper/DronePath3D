// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneManager.h"
#include "DatabaseHelper.h"
#include "FileDialogHelper.h"
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

void USceneManager::ImportScene(FString FilePath)
{
	UDatabaseHelper::GetSingleton()->AddNewScene(FilePath);
	Initialize();
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
	//for (USceneInfo* Scene : Scenes) {
	//	Scene->PointCloudData = ULidarPointCloud::CreateFromFile(Scene->PointCloudDataPath);
	//}
}
