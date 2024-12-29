// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "DroneControlTerminal.generated.h"


/**
 * 
 */
UCLASS()
class DRONEPATH3D_API UDroneControlTerminal : public UMovementComponent
{
	GENERATED_BODY()
	int point = 0;
private:
	float Speed = 1000.0f; // Ä¬ÈÏËÙ¶È
public:
	UPROPERTY(BlueprintReadWrite, Category = "Drone Control Terminal")
	TArray<FVector> TargetLocation;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)override;
};
