// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DroneInfo.generated.h"

/**
 * 
 */
UCLASS()
class DRONEPATH3D_API UDroneInfo : public UObject
{
	GENERATED_BODY()
public:
	int32 DroneID;
	int32 SceneID;
	FVector StartPosition;
	float Diameter;
	float Height;
	float MaxHeight;
	float MaxThrust;
	float MaxSpeed;
	float Weight;
	float Endurance;
	FString Name;
};
