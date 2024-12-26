// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#define STATIC_CONCPP
#include "mysql_driver.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include "SceneInfo.h"
#include "DroneInfo.h"
#include "TargetPointInfo.h"

#include "DatabaseHelper.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DRONEPATH3D_API UDatabaseHelper : public UObject
{
	GENERATED_BODY()

public:

	static UDatabaseHelper* GetSingleton();

	bool execute(FString sql);

	bool AddNewScene(FString FilePath);

	bool DeleteScene(int32 SceneID);

	TArray<USceneInfo*> GetAllScenes();

	bool AddNewDrone(const UDroneInfo* DroneInfo);

	bool DeleteDrone(int32 DroneID);

	bool UpdateDroneInfo(UDroneInfo* DroneInfo);

	TArray<UDroneInfo*> GetAllDrones();

	bool AddNewTargetPoint(const UTargetPointInfo* TargetPointInfo);

	bool DeleteTargetPoint(int32 TargetPointID);

	bool UpdateTargetPointInfo(UTargetPointInfo* TargetPointInfo);

	TArray<UTargetPointInfo*> GetAllTargetPoints();

private:
	static UDatabaseHelper* Singleton;

	sql::mysql::MySQL_Driver* driver;

	std::string HostName, UserName, Password;

	void Initialize();

};

UDatabaseHelper* UDatabaseHelper::Singleton = nullptr;
