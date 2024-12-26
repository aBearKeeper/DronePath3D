// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneInfo.h"
#include "DatabaseHelper.h"
#include "LidarPointCloud.h"

ULidarPointCloud* USceneInfo::getPointCloudData()
{
	return ULidarPointCloud::CreateFromFile(PointCloudDataPath);
}

UTargetPointInfo* USceneInfo::FindTargetPointByID(int32 PointID)
{
	for (auto Point : TargetPoints) {
		if (Point->PointID == PointID) {
			return Point;
		}
	}
	return nullptr;
}

void USceneInfo::AddTargetPoint(FVector Position)
{
	UTargetPointInfo* TargetPointInfo = NewObject<UTargetPointInfo>();
	TargetPointInfo->Position = Position;
	TargetPointInfo->SceneID = SceneID;
	UDatabaseHelper* DatabaseHelper = UDatabaseHelper::GetSingleton();
	// 存入数据库
	DatabaseHelper->AddNewTargetPoint(TargetPointInfo);
	// 从数据库更新数据
	TargetPoints = DatabaseHelper->GetSceneTargetPoints(SceneID);
}

void USceneInfo::UpdateTargetPoint(UTargetPointInfo* TargetPointInfo)
{
	UDatabaseHelper::GetSingleton()->UpdateTargetPointInfo(TargetPointInfo);
}

void USceneInfo::RemoveTargetPoint(UTargetPointInfo* TargetPointInfo)
{
	if (!TargetPointInfo)return;
	UDatabaseHelper::GetSingleton()->DeleteTargetPoint(TargetPointInfo->PointID);
	TargetPoints.RemoveSingle(TargetPointInfo);
}
