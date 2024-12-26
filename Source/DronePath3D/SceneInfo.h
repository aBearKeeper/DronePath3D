// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LidarPointCloudActor.h"
#include "TargetPointInfo.h"
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

	UFUNCTION(BlueprintCallable, Category = "Scene Info")
	ULidarPointCloud* getPointCloudData();

	UPROPERTY(BlueprintReadOnly, Category = "Scene Info")
	FString PointCloudDataPath;

	UPROPERTY(BlueprintReadOnly, Category = "Scene Info")
	TArray<UTargetPointInfo*> TargetPoints;

	UFUNCTION(BlueprintCallable, Category = "Scene Info")
	UTargetPointInfo* FindTargetPointByID(int32 PointID);

	UFUNCTION(BlueprintCallable, Category = "Scene Info")
	void AddTargetPoint(FVector Position);

	UFUNCTION(BlueprintCallable, Category = "Scene Info")
	void UpdateTargetPoint(UTargetPointInfo* TargetPointInfo);

	UFUNCTION(BlueprintCallable, Category = "Scene Info")
	void RemoveTargetPoint(UTargetPointInfo* TargetPointInfo);

private:

	UPROPERTY()
	ULidarPointCloud* PointCloudData = nullptr; //abandoned
};
