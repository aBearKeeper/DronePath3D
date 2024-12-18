// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LidarPointCloudActor.h"
#include "SceneInfo.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DRONEPATH3D_API USceneInfo : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Scene Info")
	int32 SceneID;

	UPROPERTY(BlueprintReadOnly, Category = "Scene Info")
	FString SceneName;

	UPROPERTY(BlueprintReadOnly, Category = "Scene Info")
	ULidarPointCloud* PointCloudData;

	FString PointCloudDataPath;

private:

};
