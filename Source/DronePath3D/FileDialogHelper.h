// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FileDialogHelper.generated.h"

/**
 *
 */
UCLASS()
class DRONEPATH3D_API UFileDialogHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "File Dialog")
	static FString SelectAndCopyPointCloudFile();
	
	UFUNCTION(BlueprintCallable, Category = "File Dialog")
	static bool RemovePointCloudFile(FString FilePath);
};