// Fill out your copyright notice in the Description page of Project Settings.


#include "FileDialogHelper.h"
#include "CoreMinimal.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

FString UFileDialogHelper::SelectAndCopyPointCloudFile()
{
    TArray<FString> AbsoluteOpenFileNames; // 获取的文件绝对路径
    FString ExtensionStr = TEXT("Point Cloud Files (*.las)|*.las");   // 文件类型过滤器

    // 打开文件对话框
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (!(DesktopPlatform && DesktopPlatform->OpenFileDialog(
        nullptr,
        TEXT("文件管理器"),
        FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()),
        TEXT(""),
        *ExtensionStr,
        EFileDialogFlags::None,
        AbsoluteOpenFileNames)))
    {
        UE_LOG(LogTemp, Warning, TEXT("No file selected or operation canceled."));
        return ("");
    }
    // 如果用户取消选择文件，AbsoluteOpenFileNames 会为空
    if (AbsoluteOpenFileNames.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No file selected."));
        return "";
    }

    // 定义目标文件夹路径（相对于项目目录）
    FString DestinationFolder = FPaths::ProjectDir() + TEXT("Data/PointClouds/");
    IFileManager& FileManager = IFileManager::Get();

    // 确保目标文件夹存在
    if (!FileManager.DirectoryExists(*DestinationFolder))
    {
        FileManager.MakeDirectory(*DestinationFolder);
    }

    // 遍历选中的文件
    for (const FString& Filename : AbsoluteOpenFileNames)
    {
        // 获取源文件名（不含路径部分）
        FString BaseFileName = FPaths::GetCleanFilename(Filename);

        // 拼接目标路径
        FString DestinationPath = DestinationFolder + BaseFileName;

        // 复制文件到目标路径
        if (FileManager.Copy(*DestinationPath, *Filename) == ECopyResult::COPY_OK)
        {
            UE_LOG(LogTemp, Log, TEXT("成功复制文件: %s 到 %s"), *Filename, *DestinationPath);
            return BaseFileName;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to copy: %s"), *Filename);
        }
    }
    return "";
}


bool UFileDialogHelper::RemovePointCloudFile(FString FilePath)
{
    // 获取文件管理器
    IFileManager& FileManager = IFileManager::Get();

    // 检查文件是否存在
    if (FileManager.FileExists(*FilePath))
    {
        // 删除文件
        if (FileManager.Delete(*FilePath))
        {
            UE_LOG(LogTemp, Log, TEXT("Successfully deleted: %s"), *FilePath);
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to delete: %s"), *FilePath);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("File not found: %s"), *FilePath);
    }

    return false;
}

void UFileDialogHelper::SaveWaypointsToFile(int32 RouteID, const TArray<FVector>& Waypoints)
{
    // 目标文件夹路径
    FString DestinationFolder = FPaths::ProjectDir() + TEXT("Data/Routes/");

    // 检查目标文件夹是否存在，不存在则创建
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*DestinationFolder))
    {
        PlatformFile.CreateDirectoryTree(*DestinationFolder);
    }

    // 文件名和路径
    FString FileName = FString::Printf(TEXT("%d.json"), RouteID);
    FString FilePath = DestinationFolder + FileName;

    // JSON 数组构建
    TArray<TSharedPtr<FJsonValue>> JsonWaypoints;
    for (const FVector& Point : Waypoints)
    {
        TSharedPtr<FJsonObject> PointObject = MakeShareable(new FJsonObject());
        PointObject->SetNumberField(TEXT("X"), Point.X);
        PointObject->SetNumberField(TEXT("Y"), Point.Y);
        PointObject->SetNumberField(TEXT("Z"), Point.Z);

        JsonWaypoints.Add(MakeShareable(new FJsonValueObject(PointObject)));
    }

    // 序列化 JSON 数据
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    if (FJsonSerializer::Serialize(JsonWaypoints, Writer))
    {
        // 保存到文件
        if (FFileHelper::SaveStringToFile(OutputString, *FilePath))
        {
            UE_LOG(LogTemp, Log, TEXT("Successfully saved waypoints to file: %s"), *FilePath);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to save waypoints to file: %s"), *FilePath);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to serialize waypoints to JSON format."));
    }
}

bool UFileDialogHelper::LoadWaypointsFromFile(int32 RouteID, TArray<FVector>& OutWaypoints)
{
    // 清空输出数组
    OutWaypoints.Empty();

    // 文件路径
    FString DestinationFolder = FPaths::ProjectDir() + TEXT("Data/Routes/");
    FString FileName = FString::Printf(TEXT("%d.json"), RouteID);
    FString FilePath = DestinationFolder + FileName;

    // 检查文件是否存在
    if (!FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("File not found: %s"), *FilePath);
        return false;
    }

    // 读取文件内容
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read file: %s"), *FilePath);
        return false;
    }

    // 解析 JSON 数据
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);
    TArray<TSharedPtr<FJsonValue>> JsonWaypoints;
    if (FJsonSerializer::Deserialize(Reader, JsonWaypoints))
    {
        for (const TSharedPtr<FJsonValue>& JsonValue : JsonWaypoints)
        {
            // 确保值是对象类型
            if (JsonValue->Type == EJson::Object)
            {
                TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
                if (JsonObject.IsValid())
                {
                    // 提取 X, Y, Z 值
                    float X = JsonObject->GetNumberField(TEXT("X"));
                    float Y = JsonObject->GetNumberField(TEXT("Y"));
                    float Z = JsonObject->GetNumberField(TEXT("Z"));

                    // 添加到输出数组
                    OutWaypoints.Add(FVector(X, Y, Z));
                }
            }
        }

        UE_LOG(LogTemp, Log, TEXT("Successfully loaded waypoints from file: %s"), *FilePath);
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON from file: %s"), *FilePath);
        return false;
    }
}
