// Fill out your copyright notice in the Description page of Project Settings.


#include "DatabaseHelper.h"

UDatabaseHelper* UDatabaseHelper::GetSingleton()
{
	if (!Singleton) {
		Singleton = NewObject<UDatabaseHelper>();
		Singleton->AddToRoot();
		Singleton->Initialize();
	}
	return Singleton;
}

bool UDatabaseHelper::execute(FString sql)
{
    try {
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));
        std::unique_ptr<sql::Statement> stamt(con->createStatement());
        stamt->execute("USE DronePath3D");
        stamt->execute(TCHAR_TO_UTF8(*sql));
        return true;
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
    return false;
}

bool UDatabaseHelper::AddNewScene(FString FilePath)
{
    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 获取文件名作为 Name
        std::string sceneName = TCHAR_TO_UTF8(*FPaths::GetBaseFilename(FilePath));

        // 1. 检查是否已经存在相同的 Name
        std::string checkQuery = "SELECT COUNT(*) FROM Scenes WHERE Name = ?";
        std::unique_ptr<sql::PreparedStatement> checkStmt(con->prepareStatement(checkQuery));
        checkStmt->setString(1, sceneName);

        // 执行查询
        std::unique_ptr<sql::ResultSet> res(checkStmt->executeQuery());
        res->next();

        // 如果结果是 1，则表示已经存在相同的 Name，返回 false
        if (res->getInt(1) > 0) {
            UE_LOG(LogTemp, Warning, TEXT("Scene with name '%s' already exists."), *FString(sceneName.c_str()));
            return false;
        }

        // 2. 如果没有重复的 Name，执行插入操作
        std::string query = "INSERT INTO Scenes (Name, Description, PointCloudDataPath) VALUES (?, ?, ?)";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

        // 设置参数
        pstmt->setString(1, sceneName); // 设置 Name
        pstmt->setString(2, "description"); // 设置 Description
        pstmt->setString(3, TCHAR_TO_UTF8(*(FPaths::ProjectDir() + TEXT("Data/PointClouds/") + FilePath))); // 设置 PointCloudDataPath

        // 执行插入
        pstmt->executeUpdate();

        return true;
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
    return false;
}

bool UDatabaseHelper::DeleteScene(int32 SceneID)
{
    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 创建删除语句
        std::string query = "DELETE FROM Scenes WHERE SceneID = ?";

        // 使用预处理语句
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

        // 设置参数
        pstmt->setInt(1, SceneID);

        // 执行删除操作
        int rowsAffected = pstmt->executeUpdate();

        // 检查是否有记录被删除
        if (rowsAffected > 0) {
            UE_LOG(LogTemp, Log, TEXT("Scene with ID %d deleted successfully."), SceneID);
            return true;
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("No scene found with ID %d."), SceneID);
        }
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
    return false;
}

TArray<USceneInfo*> UDatabaseHelper::GetAllScenes()
{
    TArray<USceneInfo*> scenes;
    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 执行查询语句
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM Scenes"));

        // 遍历结果集
        while (res->next()) {
            // 创建一个新的 SceneInfo 对象
            USceneInfo* sceneInfo = NewObject<USceneInfo>();

            // 获取查询结果并填充 USceneInfo 对象
            sceneInfo->SceneID = res->getInt("SceneID");
            sceneInfo->SceneName = FString(res->getString("Name").c_str());
            sceneInfo->PointCloudDataPath = FString(res->getString("PointCloudDataPath").c_str());

            // 将 SceneInfo 对象添加到 TArray
            scenes.Add(sceneInfo);
        }

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

    return scenes;
}

bool UDatabaseHelper::AddNewDrone(const UDroneInfo* DroneInfo)
{
    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 1. 检查是否已经存在相同的 Name 和 SceneID
        std::string checkQuery = "SELECT COUNT(*) FROM drones WHERE Name = ? AND SceneID = ?";
        std::unique_ptr<sql::PreparedStatement> checkStmt(con->prepareStatement(checkQuery));
        checkStmt->setString(1, TCHAR_TO_UTF8(*DroneInfo->Name));  // 设置 Name
        checkStmt->setInt(2, DroneInfo->SceneID);                   // 设置 SceneID

        // 执行查询
        std::unique_ptr<sql::ResultSet> res(checkStmt->executeQuery());
        res->next();

        // 如果结果大于 0，表示该组合已经存在，返回 false
        if (res->getInt(1) > 0) {
            UE_LOG(LogTemp, Warning, TEXT("Drone with name '%s' and SceneID '%d' already exists."), *DroneInfo->Name, DroneInfo->SceneID);
            return false;
        }

        // 2. 如果没有重复的 Name，执行插入操作
        std::string query = "INSERT INTO drones (SceneID, Name, MaxSpeed, MaxHeight, Endurance, Weight, MaxThrust, Diameter, Height, StartX, StartY, StartZ) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

        // 设置参数
        pstmt->setInt(1, DroneInfo->SceneID);                     // 设置 SceneID
        pstmt->setString(2, TCHAR_TO_UTF8(*DroneInfo->Name));     // 设置 Name
        pstmt->setDouble(3, DroneInfo->MaxSpeed);                 // 设置 MaxSpeed
        pstmt->setDouble(4, DroneInfo->MaxHeight);                // 设置 MaxHeight
        pstmt->setDouble(5, DroneInfo->Endurance);                // 设置 Endurance
        pstmt->setDouble(6, DroneInfo->Weight);                   // 设置 Weight
        pstmt->setDouble(7, DroneInfo->MaxThrust);                // 设置 MaxThrust
        pstmt->setDouble(8, DroneInfo->Diameter);                 // 设置 Diameter
        pstmt->setDouble(9, DroneInfo->Height);                   // 设置 Height
        pstmt->setDouble(10, DroneInfo->StartPosition.X);         // 设置 StartX
        pstmt->setDouble(11, DroneInfo->StartPosition.Y);         // 设置 StartY
        pstmt->setDouble(12, DroneInfo->StartPosition.Z);         // 设置 StartZ

        // 执行插入
        pstmt->executeUpdate();

        return true;
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

    return false;
}

bool UDatabaseHelper::DeleteDrone(int32 DroneID)
{
    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 执行删除操作
        std::string query = "DELETE FROM drones WHERE DroneID = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

        // 设置参数
        pstmt->setInt(1, DroneID);

        // 执行删除
        pstmt->executeUpdate();

        return true;
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

    return false;
}

bool UDatabaseHelper::UpdateDroneInfo(UDroneInfo* DroneInfo)
{
    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 1. 检查是否存在该 DroneID
        std::string checkQuery1 = "SELECT COUNT(*) FROM drones WHERE DroneID = ?";
        std::unique_ptr<sql::PreparedStatement> checkStmt1(con->prepareStatement(checkQuery1));
        checkStmt1->setInt(1, DroneInfo->DroneID);

        // 执行查询
        std::unique_ptr<sql::ResultSet> res1(checkStmt1->executeQuery());
        res1->next();

        // 如果没有找到该 DroneID，返回 false
        if (res1->getInt(1) == 0) {
            UE_LOG(LogTemp, Warning, TEXT("Drone with ID '%d' does not exist."), DroneInfo->DroneID);
            return false;
        }

        // 2. 检查是否已经存在相同的 Name 和 SceneID
        std::string checkQuery2 = "SELECT COUNT(*) FROM drones WHERE Name = ? AND SceneID = ? AND DroneID != ?";
        std::unique_ptr<sql::PreparedStatement> checkStmt2(con->prepareStatement(checkQuery2));
        checkStmt2->setString(1, TCHAR_TO_UTF8(*DroneInfo->Name));  // 设置 Name
        checkStmt2->setInt(2, DroneInfo->SceneID);                   // 设置 SceneID
        checkStmt2->setInt(3, DroneInfo->DroneID);                   // 设置 DroneID

        // 执行查询
        std::unique_ptr<sql::ResultSet> res2(checkStmt2->executeQuery());
        res2->next();

        // 如果结果大于 0，表示该组合已经存在，返回 false
        if (res2->getInt(1) > 0) {
            UE_LOG(LogTemp, Warning, TEXT("Drone with name '%s' and SceneID '%d' already exists."), *DroneInfo->Name, DroneInfo->SceneID);
            return false;
        }

        // 3. 如果找到该 DroneID，执行更新操作
        std::string query = "UPDATE drones SET SceneID = ?, Name = ?, MaxSpeed = ?, MaxHeight = ?, Endurance = ?, "
            "Weight = ?, MaxThrust = ?, Diameter = ?, Height = ?, StartX = ?, StartY = ?, StartZ = ? WHERE DroneID = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

        // 设置参数
        pstmt->setInt(1, DroneInfo->SceneID);                     // 设置 SceneID
        pstmt->setString(2, TCHAR_TO_UTF8(*DroneInfo->Name));     // 设置 Name
        pstmt->setDouble(3, DroneInfo->MaxSpeed);                 // 设置 MaxSpeed
        pstmt->setDouble(4, DroneInfo->MaxHeight);                // 设置 MaxHeight
        pstmt->setDouble(5, DroneInfo->Endurance);                // 设置 Endurance
        pstmt->setDouble(6, DroneInfo->Weight);                   // 设置 Weight
        pstmt->setDouble(7, DroneInfo->MaxThrust);                // 设置 MaxThrust
        pstmt->setDouble(8, DroneInfo->Diameter);                 // 设置 Diameter
        pstmt->setDouble(9, DroneInfo->Height);                   // 设置 Height
		pstmt->setDouble(10, DroneInfo->StartPosition.X);         // 设置 StartX
        pstmt->setDouble(11, DroneInfo->StartPosition.Y);         // 设置 StartY
        pstmt->setDouble(12, DroneInfo->StartPosition.Z);         // 设置 StartZ
        pstmt->setInt(13, DroneInfo->DroneID);                    // 设置 DroneID

        // 执行更新
        pstmt->executeUpdate();

        return true;
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

    return false;
}

TArray<UDroneInfo*> UDatabaseHelper::GetAllDrones()
{
    TArray<UDroneInfo*> drones;
    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 执行查询语句
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM drones"));

        // 遍历结果集
        while (res->next()) {
            // 创建一个新的 UDroneInfo 对象
            UDroneInfo* droneInfo = NewObject<UDroneInfo>();

            // 获取查询结果并填充 UDroneInfo 对象
            droneInfo->DroneID = res->getInt("DroneID");
            droneInfo->SceneID = res->getInt("SceneID");
            droneInfo->Name = FString(res->getString("Name").c_str());
            droneInfo->MaxSpeed = res->getDouble("MaxSpeed");
            droneInfo->MaxHeight = res->getDouble("MaxHeight");
            droneInfo->Endurance = res->getDouble("Endurance");
            droneInfo->Weight = res->getDouble("Weight");
            droneInfo->MaxThrust = res->getDouble("MaxThrust");
            droneInfo->Diameter = res->getDouble("Diameter");
            droneInfo->Height = res->getDouble("Height");
            droneInfo->StartPosition = FVector(
                res->getDouble("StartX"),
                res->getDouble("StartY"),
                res->getDouble("StartZ")
            );

            // 将 UDroneInfo 对象添加到 TArray
            drones.Add(droneInfo);
        }
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

    return drones;
}

bool UDatabaseHelper::AddNewTargetPoint(const UTargetPointInfo* TargetPointInfo)
{
    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 执行插入操作
        std::string query = "INSERT INTO TargetPoints (SceneID, DroneID, CoordinateX, CoordinateY, CoordinateZ) "
            "VALUES (?, ?, ?, ?, ?)";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

        // 设置参数
        pstmt->setInt(1, TargetPointInfo->SceneID);               // 设置 SceneID
        pstmt->setInt(2, TargetPointInfo->AssignedDroneID);        // 设置 DroneID
        pstmt->setDouble(3, TargetPointInfo->Position.X);          // 设置 CoordinateX
        pstmt->setDouble(4, TargetPointInfo->Position.Y);          // 设置 CoordinateY
        pstmt->setDouble(5, TargetPointInfo->Position.Z);          // 设置 CoordinateZ

        // 执行插入
        pstmt->executeUpdate();

        return true;
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

    return false;
}

bool UDatabaseHelper::DeleteTargetPoint(int32 TargetPointID)
{
    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 执行删除操作
        std::string query = "DELETE FROM TargetPoints WHERE TargetPointID = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

        // 设置参数
        pstmt->setInt(1, TargetPointID);

        // 执行删除
        pstmt->executeUpdate();

        return true;
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

    return false;
}

bool UDatabaseHelper::UpdateTargetPointInfo(UTargetPointInfo* TargetPointInfo)
{
    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 1. 检查是否存在该 TargetPointID
        std::string checkQuery = "SELECT COUNT(*) FROM TargetPoints WHERE TargetPointID = ?";
        std::unique_ptr<sql::PreparedStatement> checkStmt(con->prepareStatement(checkQuery));
        checkStmt->setInt(1, TargetPointInfo->PointID);

        // 执行查询
        std::unique_ptr<sql::ResultSet> res(checkStmt->executeQuery());
        res->next();

        // 如果没有找到该 TargetPointID，返回 false
        if (res->getInt(1) == 0) {
            UE_LOG(LogTemp, Warning, TEXT("TargetPoint with ID '%d' does not exist."), TargetPointInfo->PointID);
            return false;
        }

        // 2. 如果找到该 TargetPointID，执行更新操作
        std::string query = "UPDATE TargetPoints SET SceneID = ?, DroneID = ?, CoordinateX = ?, CoordinateY = ?, CoordinateZ = ? "
            "WHERE TargetPointID = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

        // 设置参数
        pstmt->setInt(1, TargetPointInfo->SceneID);                // 设置 SceneID
        pstmt->setInt(2, TargetPointInfo->AssignedDroneID);         // 设置 DroneID
        pstmt->setDouble(3, TargetPointInfo->Position.X);           // 设置 CoordinateX
        pstmt->setDouble(4, TargetPointInfo->Position.Y);           // 设置 CoordinateY
        pstmt->setDouble(5, TargetPointInfo->Position.Z);           // 设置 CoordinateZ
        pstmt->setInt(6, TargetPointInfo->PointID);                 // 设置 TargetPointID

        // 执行更新
        pstmt->executeUpdate();

        return true;
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

    return false;
}




TArray<UTargetPointInfo*> UDatabaseHelper::GetAllTargetPoints()
{
    TArray<UTargetPointInfo*> points;

    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 执行查询语句
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM TargetPoints"));

        // 遍历结果集
        while (res->next()) {
            // 创建一个新的 UTargetPointInfo 对象
            UTargetPointInfo* pointInfo = NewObject<UTargetPointInfo>();

            // 获取查询结果并填充 UTargetPointInfo 对象
            pointInfo->PointID = res->getInt("TargetPointID");
            pointInfo->SceneID = res->getInt("SceneID");
            pointInfo->AssignedDroneID = res->getInt("DroneID");
            pointInfo->Position = FVector(
                res->getDouble("CoordinateX"),
                res->getDouble("CoordinateY"),
                res->getDouble("CoordinateZ")
            );

            // 将 UDroneInfo 对象添加到 TArray
            points.Add(pointInfo);
        }
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

    return points;
}

TArray<UTargetPointInfo*> UDatabaseHelper::GetSceneTargetPoints(int32 SceneID)
{
    TArray<UTargetPointInfo*> points;

    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 创建查询语句
        std::string query = "SELECT * FROM TargetPoints WHERE SceneID = ?";

        // 准备预处理语句
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

        // 设置参数
        pstmt->setInt(1, SceneID);

        // 执行查询语句
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        // 遍历结果集
        while (res->next()) {
            // 创建一个新的 UTargetPointInfo 对象
            UTargetPointInfo* pointInfo = NewObject<UTargetPointInfo>();

            // 获取查询结果并填充 UTargetPointInfo 对象
            pointInfo->PointID = res->getInt("TargetPointID");
            pointInfo->SceneID = res->getInt("SceneID");
            pointInfo->AssignedDroneID = res->getInt("DroneID");
            pointInfo->Position = FVector(
                res->getDouble("CoordinateX"),
                res->getDouble("CoordinateY"),
                res->getDouble("CoordinateZ")
            );

            // 将 UTargetPointInfo 对象添加到 TArray
            points.Add(pointInfo);
        }
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

    return points;
}

bool UDatabaseHelper::AddNewRoutePlan(URoutePlan* Plan)
{
    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 插入路径组（RoutePlan）数据
        std::string insertRoutePlanQuery = "INSERT INTO RoutePlans (SceneID) VALUES (?)";
        std::unique_ptr<sql::PreparedStatement> pstmtRoutePlan(con->prepareStatement(insertRoutePlanQuery));
        pstmtRoutePlan->setInt(1, Plan->SceneID);  // 设置 SceneID
        pstmtRoutePlan->executeUpdate();

        // 获取新插入路径组的 PlanID
        std::unique_ptr<sql::ResultSet> resultSet1(stmt->executeQuery("SELECT LAST_INSERT_ID() AS PlanID"));
        if (resultSet1->next()) {
            Plan->PlanID = resultSet1->getInt("PlanID");
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Failed to retrieve PlanID for the new RoutePlan."));
            return false;
        }

        // 插入路径（Routes）数据
        std::string insertRouteQuery =
            "INSERT INTO Routes (RoutePlanID, DroneID, Status) VALUES (?, ?, ?)";
        std::unique_ptr<sql::PreparedStatement> pstmtRoute(con->prepareStatement(insertRouteQuery));

        for (URoute* Route : Plan->Routes) {
            pstmtRoute->setInt(1, Plan->PlanID);                   // 设置 RoutePlanID
            pstmtRoute->setInt(2, Route->DroneID);                // 设置 DroneID
            pstmtRoute->setString(3, "Planning");                 // 设置默认状态为 "Planning"
            pstmtRoute->executeUpdate();

            // 获取新插入路径的 RouteID
            std::unique_ptr<sql::ResultSet> resultSet2(stmt->executeQuery("SELECT LAST_INSERT_ID() AS RouteID"));
            if (resultSet2->next()) {
                Route->RouteID = resultSet2->getInt("RouteID");
                Route->PlanID = Plan->PlanID;  // 同步 PlanID 到每个路径
            }
            else {
                UE_LOG(LogTemp, Error, TEXT("Failed to retrieve RouteID for a new Route."));
                return false;
            }
        }

        UE_LOG(LogTemp, Log, TEXT("Successfully added RoutePlan with PlanID: %d"), Plan->PlanID);
        return true;
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

    return false;
}

bool UDatabaseHelper::DeleteRoutePlan(int32 PlanID)
{
    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 删除路径组
        std::string deleteRoutePlanQuery = "DELETE FROM RoutePlans WHERE RoutePlanID = ?";
        std::unique_ptr<sql::PreparedStatement> pstmtDeleteRoutePlan(con->prepareStatement(deleteRoutePlanQuery));
        pstmtDeleteRoutePlan->setInt(1, PlanID);
        pstmtDeleteRoutePlan->executeUpdate();

        return true;
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

    return false;
}


TArray<URoutePlan*> UDatabaseHelper::GetAllRoutePlans()
{
    TArray<URoutePlan*> Plans;

    try {
        // 创建数据库连接
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));

        // 选择数据库
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("USE DronePath3D");

        // 查询所有路径组信息
        std::unique_ptr<sql::ResultSet> routePlanRes(stmt->executeQuery("SELECT * FROM RoutePlans"));

        while (routePlanRes->next()) {
            // 创建一个新的 URoutePlan 对象
            URoutePlan* RoutePlan = NewObject<URoutePlan>();
            RoutePlan->PlanID = routePlanRes->getInt("RoutePlanID");
            RoutePlan->SceneID = routePlanRes->getInt("SceneID");

            // 查询当前路径组的所有路径
            std::string query = "SELECT * FROM Routes WHERE RoutePlanID = ?";
            std::unique_ptr<sql::PreparedStatement> pstmtRoutes(con->prepareStatement(query));
            pstmtRoutes->setInt(1, RoutePlan->PlanID);
            std::unique_ptr<sql::ResultSet> routeRes(pstmtRoutes->executeQuery());

            while (routeRes->next()) {
                // 创建一个新的 URoute 对象
                URoute* Route = NewObject<URoute>();
                Route->RouteID = routeRes->getInt("RouteID");
                Route->PlanID = routeRes->getInt("RoutePlanID");
                Route->DroneID = routeRes->getInt("DroneID");

                // 添加路径到路径组
                RoutePlan->Routes.Add(Route);
            }

            // 添加路径组到 Plans
            Plans.Add(RoutePlan);
        }
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

    return Plans;
}


void UDatabaseHelper::Initialize()
{
    FString Host, User, Pass;

    // 从配置文件中读取
    if (GConfig)
    {
        GConfig->GetString(TEXT("DatabaseSettings"), TEXT("Host"), Host, GGameIni);
        GConfig->GetString(TEXT("DatabaseSettings"), TEXT("User"), User, GGameIni);
        GConfig->GetString(TEXT("DatabaseSettings"), TEXT("Password"), Pass, GGameIni);
        this->HostName = TCHAR_TO_UTF8(*Host);
        this->UserName = TCHAR_TO_UTF8(*User);
        this->Password = TCHAR_TO_UTF8(*Pass);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GConfig is not available!"));
    }
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect(HostName, UserName, Password));
        std::unique_ptr<sql::Statement> stamt(con->createStatement());
        stamt->execute("CREATE DATABASE IF NOT EXISTS DronePath3D");
        stamt->execute("USE DronePath3D");
        stamt->execute("CREATE TABLE IF NOT EXISTS Scenes ("
            "SceneID INT AUTO_INCREMENT PRIMARY KEY,"
            "Name VARCHAR(255) NOT NULL,"
            "Description VARCHAR(500),"
            "PointCloudDataPath VARCHAR(1000)"
            "); ");
        stamt->execute("CREATE TABLE IF NOT EXISTS Drones ("
            "DroneID INT AUTO_INCREMENT PRIMARY KEY,"
            "SceneID INT,"
            "StartX FLOAT,"
            "StartY FLOAT,"
            "StartZ FLOAT,"
            "Name VARCHAR(255),"
            "MaxSpeed FLOAT,"
            "MaxHeight FLOAT,"
            "Endurance FLOAT,"
            "Weight FLOAT,"
            "MaxThrust FLOAT,"
            "Diameter FLOAT,"
            "Height FLOAT,"
            "FOREIGN KEY(SceneID) REFERENCES Scenes(SceneID) ON DELETE CASCADE"
            "); ");
        stamt->execute("CREATE TABLE IF NOT EXISTS TargetPoints ("
            "TargetPointID INT AUTO_INCREMENT PRIMARY KEY,"
            "SceneID INT,"
            "DroneID INT,"
            "CoordinateX FLOAT,"
            "CoordinateY FLOAT,"
            "CoordinateZ FLOAT,"
            "FOREIGN KEY(SceneID) REFERENCES Scenes(SceneID) ON DELETE CASCADE"
            "); ");
        stamt->execute("CREATE TABLE IF NOT EXISTS RoutePlans ("
            "RoutePlanID INT AUTO_INCREMENT PRIMARY KEY,"
            "SceneID INT,"
            "FOREIGN KEY(SceneID) REFERENCES Scenes(SceneID) ON DELETE CASCADE"
            "); ");
        stamt->execute("CREATE TABLE IF NOT EXISTS Routes ("
            "RouteID INT AUTO_INCREMENT PRIMARY KEY,"
            "RoutePlanID INT,"
            "DroneID INT,"
            "Status VARCHAR(255),"
            "FOREIGN KEY(RoutePlanID) REFERENCES RoutePlans(RoutePlanID) ON DELETE CASCADE,"
            "FOREIGN KEY(DroneID) REFERENCES Drones(DroneID) ON DELETE SET NULL"
            "); ");

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
