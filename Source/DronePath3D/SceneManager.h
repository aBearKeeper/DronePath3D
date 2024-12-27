// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SceneInfo.h"
#include "SceneManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DRONEPATH3D_API USceneManager : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "Scene Manager")
	TArray<USceneInfo*> Scenes;

	UPROPERTY(BlueprintReadWrite, Category = "Scene Manager")
	USceneInfo* CurrentScene;

	UFUNCTION(BlueprintPure, Category = "Scene Manager")
	static USceneManager* GetSingleton();

	UFUNCTION(BlueprintCallable, Category = "Scene Manager")
	bool ImportScene(FString FilePath);

	UFUNCTION(BlueprintCallable, Category = "Scene Manager")
	void DeleteScene(int32 ID, FString FilePath);

	void DroneRemovalNotice(int32 SceneID, int32 DroneID);

	void Initialize();

private:
	static USceneManager* Singleton;
};

USceneManager* USceneManager::Singleton = nullptr;
