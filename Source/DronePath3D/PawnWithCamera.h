// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnWithCamera.generated.h"

UCLASS()
class DRONEPATH3D_API APawnWithCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnWithCamera();

protected:

	//输入变量
	FVector MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	bool bZoomingIn;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere)
	float MaxFieldOfView;

	UPROPERTY(EditAnywhere)
	float MinFieldOfView;

	UPROPERTY(EditAnywhere)
	float MaxTargetArmLength;

	UPROPERTY(EditAnywhere)
	float MinTargetArmLength;

	UPROPERTY(EditAnywhere)
	float MaxPitch;

	UPROPERTY(EditAnywhere)
	float MinPitch;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 输入函数
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveUp(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void ZoomIn();
	void ZoomOut();

};
