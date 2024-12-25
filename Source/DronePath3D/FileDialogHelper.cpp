// Fill out your copyright notice in the Description page of Project Settings.


#include "FileDialogHelper.h"
#include "CoreMinimal.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Misc/Paths.h"


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