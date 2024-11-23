// Fill out your copyright notice in the Description page of Project Settings.


#include "FileDialogHelper.h"
#include "CoreMinimal.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Misc/Paths.h"


void UFileDialogHelper::SelectAndCopyFile()
{
    TArray<FString> AbsoluteOpenFileNames; // ��ȡ���ļ�����·��
    FString ExtensionStr = TEXT("*.*");   // �ļ�����

    // ���ļ��Ի���
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (!(DesktopPlatform && DesktopPlatform->OpenFileDialog(
        nullptr,
        TEXT("�ļ�������"),
        FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()),
        TEXT(""),
        *ExtensionStr,
        EFileDialogFlags::None,
        AbsoluteOpenFileNames)))
    {
        UE_LOG(LogTemp, Warning, TEXT("No file selected or operation canceled."));
        return;
    }
    // ����Ŀ���ļ���·�����������ĿĿ¼��
    FString DestinationFolder = FPaths::ProjectDir() + TEXT("Data/Scenes/");
    IFileManager& FileManager = IFileManager::Get();

    // ȷ��Ŀ���ļ��д���
    if (!FileManager.DirectoryExists(*DestinationFolder))
    {
        FileManager.MakeDirectory(*DestinationFolder);
    }

    // ����ѡ�е��ļ�
    for (const FString& Filename : AbsoluteOpenFileNames)
    {
        // ��ȡԴ�ļ���������·�����֣�
        FString BaseFileName = FPaths::GetCleanFilename(Filename);

        // ƴ��Ŀ��·��
        FString DestinationPath = DestinationFolder + BaseFileName;

        // �����ļ���Ŀ��·��
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
