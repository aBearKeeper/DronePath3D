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

	UFUNCTION(BlueprintCallable, Category = "DatabaseHelper")
	static UDatabaseHelper* GetSingleton();

	UFUNCTION(BlueprintCallable, Category = "DatabaseHelper")
	bool execute(FString sql);

	UFUNCTION(BlueprintCallable, Category = "DatabaseHelper")
	bool AddNewScene(FString FilePath);

	UFUNCTION(BlueprintCallable, Category = "DatabaseHelper")
	bool DeleteScene(int32 SceneID);

	UFUNCTION(BlueprintCallable, Category = "DatabaseHelper")
	TArray<USceneInfo*> GetAllScenes();

	UFUNCTION(BlueprintCallable, Category = "DatabaseHelper")
	bool AddNewDrone(const UDroneInfo* DroneInfo);

	UFUNCTION(BlueprintCallable, Category = "DatabaseHelper")
	bool DeleteDrone(int32 DroneID);

	UFUNCTION(BlueprintCallable, Category = "DatabaseHelper")
	bool UpdateDroneInfo(UDroneInfo* DroneInfo);

	UFUNCTION(BlueprintCallable, Category = "DatabaseHelper")
	TArray<UDroneInfo*> GetAllDrones();

	UFUNCTION(BlueprintCallable, Category = "DatabaseHelper")
	bool AddNewTargetPoint(const UTargetPointInfo* TargetPointInfo);

	UFUNCTION(BlueprintCallable, Category = "DatabaseHelper")
	bool DeleteTargetPoint(int32 TargetPointID);

	UFUNCTION(BlueprintCallable, Category = "DatabaseHelper")
	bool UpdateTargetPointInfo(UTargetPointInfo* TargetPointInfo);

	UFUNCTION(BlueprintCallable, Category = "DatabaseHelper")
	TArray<UTargetPointInfo*> GetAllTargetPoints();

private:
	static UDatabaseHelper* Singleton;

	sql::mysql::MySQL_Driver* driver;

	std::string HostName, UserName, Password;

	void Initialize();

};

UDatabaseHelper* UDatabaseHelper::Singleton = nullptr;
