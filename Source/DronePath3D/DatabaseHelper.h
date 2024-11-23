// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#define STATIC_CONCPP
#include "mysql_driver.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/driver.h"
#include "cppconn/exception.h"

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
	void execute(FString sql);

private:

	static UDatabaseHelper* Singleton;

	sql::mysql::MySQL_Driver* driver;

	void Initialize();

	UDatabaseHelper();

};

UDatabaseHelper* UDatabaseHelper::Singleton = nullptr;
