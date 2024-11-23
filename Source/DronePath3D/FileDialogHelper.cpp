// Fill out your copyright notice in the Description page of Project Settings.


#include "FileDialogHelper.h"
#include "CoreMinimal.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Misc/Paths.h"


void UFileDialogHelper::SelectAndCopyFile()
{
    TArray<FString> AbsoluteOpenFileNames; // 获取的文件绝对路径
    FString ExtensionStr = TEXT("*.*");   // 文件类型

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
        return;
    }
    // 定义目标文件夹路径（相对于项目目录）
    FString DestinationFolder = FPaths::ProjectDir() + TEXT("Data/Scenes/");
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
            UE_LOG(LogTemp, Log, TEXT("Successfully copied: %s to %s"), *Filename, *DestinationPath);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to copy: %s"), *Filename);
        }
    }
}
