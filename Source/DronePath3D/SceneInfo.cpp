// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneInfo.h"
#include "LidarPointCloud.h"

ULidarPointCloud* USceneInfo::getPointCloudData()
{
	if (!PointCloudData) {
		UE_LOG(LogTemp, Error, TEXT("PointCloudData is null"));
		PointCloudData = ULidarPointCloud::CreateFromFile(PointCloudDataPath);
	}
	UE_LOG(LogTemp, Error, TEXT("return PointCloudData"));
	return PointCloudData;
}
