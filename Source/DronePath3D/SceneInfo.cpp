// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneInfo.h"
#include "LidarPointCloud.h"

ULidarPointCloud* USceneInfo::getPointCloudData()
{
	if (!PointCloudData) {
		PointCloudData = ULidarPointCloud::CreateFromFile(PointCloudDataPath);
	}
	return PointCloudData;
}
