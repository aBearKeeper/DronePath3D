// Fill out your copyright notice in the Description page of Project Settings.


#include "DatabaseHelper.h"

#define HOSTNAME "tcp://localhost:3306"
#define USERNAME "root"
#define PASSWORD "WXY@030531wxy"

UDatabaseHelper* UDatabaseHelper::GetSingleton()
{
	if (!Singleton) {
		Singleton = NewObject<UDatabaseHelper>();
		Singleton->AddToRoot();
		Singleton->Initialize();
	}
	return Singleton;
}

void UDatabaseHelper::execute(FString sql)
{
    try {
        std::unique_ptr<sql::Connection> con(driver->connect(HOSTNAME, USERNAME, PASSWORD));
        std::unique_ptr<sql::Statement> stamt(con->createStatement());
        stamt->execute("USE DronePath3D");
        stamt->execute(TCHAR_TO_UTF8(*sql));
    }
    catch (const sql::SQLException& e) {
        UE_LOG(LogTemp, Error, TEXT("SQL Exception: %s"), *FString(e.what()));
    }
    catch (const std::exception& e) {
        UE_LOG(LogTemp, Error, TEXT("Standard Exception: %s"), *FString(e.what()));
    }
    catch (...) {
        UE_LOG(LogTemp, Error, TEXT("SQL execute: Unknown Exception occurred."));
    }
}

void UDatabaseHelper::Initialize()
{
	driver = sql::mysql::get_mysql_driver_instance();
    std::unique_ptr<sql::Connection> con(driver->connect(HOSTNAME, USERNAME, PASSWORD));
    std::unique_ptr<sql::Statement> stamt(con->createStatement());
    stamt->execute("CREATE DATABASE IF NOT EXISTS DronePath3D");
    stamt->execute("USE DronePath3D");
}

UDatabaseHelper::UDatabaseHelper() {

}

#undef HOSTNAME
#undef USERNAME
#undef PASSWORD
