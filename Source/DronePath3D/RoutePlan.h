// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Route.h"
#include "RoutePlan.generated.h"

/**
 * 
 */
UCLASS()
class DRONEPATH3D_API URoutePlan : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Route Plan")
	int32 PlanID;
	UPROPERTY(BlueprintReadOnly, Category = "Route Plan")
	int32 SceneID;
	UPROPERTY(BlueprintReadWrite, Category = "Route Plan")
	TArray<URoute*> Routes;
};
