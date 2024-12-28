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
    TArray<FString> AbsoluteOpenFileNames; // ��ȡ���ļ�����·��
    FString ExtensionStr = TEXT("Point Cloud Files (*.las)|*.las");   // �ļ����͹�����

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
        return ("");
    }
    // ����û�ȡ��ѡ���ļ���AbsoluteOpenFileNames ��Ϊ��
    if (AbsoluteOpenFileNames.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No file selected."));
        return "";
    }

    // ����Ŀ���ļ���·�����������ĿĿ¼��
    FString DestinationFolder = FPaths::ProjectDir() + TEXT("Data/PointClouds/");
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
            UE_LOG(LogTemp, Log, TEXT("�ɹ������ļ�: %s �� %s"), *Filename, *DestinationPath);
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
    // ��ȡ�ļ�������
    IFileManager& FileManager = IFileManager::Get();

    // ����ļ��Ƿ����
    if (FileManager.FileExists(*FilePath))
    {
        // ɾ���ļ�
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
    // Ŀ���ļ���·��
    FString DestinationFolder = FPaths::ProjectDir() + TEXT("Data/Routes/");

    // ���Ŀ���ļ����Ƿ���ڣ��������򴴽�
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*DestinationFolder))
    {
        PlatformFile.CreateDirectoryTree(*DestinationFolder);
    }

    // �ļ�����·��
    FString FileName = FString::Printf(TEXT("%d.json"), RouteID);
    FString FilePath = DestinationFolder + FileName;

    // JSON ���鹹��
    TArray<TSharedPtr<FJsonValue>> JsonWaypoints;
    for (const FVector& Point : Waypoints)
    {
        TSharedPtr<FJsonObject> PointObject = MakeShareable(new FJsonObject());
        PointObject->SetNumberField(TEXT("X"), Point.X);
        PointObject->SetNumberField(TEXT("Y"), Point.Y);
        PointObject->SetNumberField(TEXT("Z"), Point.Z);

        JsonWaypoints.Add(MakeShareable(new FJsonValueObject(PointObject)));
    }

    // ���л� JSON ����
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    if (FJsonSerializer::Serialize(JsonWaypoints, Writer))
    {
        // ���浽�ļ�
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
    // ����������
    OutWaypoints.Empty();

    // �ļ�·��
    FString DestinationFolder = FPaths::ProjectDir() + TEXT("Data/Routes/");
    FString FileName = FString::Printf(TEXT("%d.json"), RouteID);
    FString FilePath = DestinationFolder + FileName;

    // ����ļ��Ƿ����
    if (!FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("File not found: %s"), *FilePath);
        return false;
    }

    // ��ȡ�ļ�����
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read file: %s"), *FilePath);
        return false;
    }

    // ���� JSON ����
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);
    TArray<TSharedPtr<FJsonValue>> JsonWaypoints;
    if (FJsonSerializer::Deserialize(Reader, JsonWaypoints))
    {
        for (const TSharedPtr<FJsonValue>& JsonValue : JsonWaypoints)
        {
            // ȷ��ֵ�Ƕ�������
            if (JsonValue->Type == EJson::Object)
            {
                TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
                if (JsonObject.IsValid())
                {
                    // ��ȡ X, Y, Z ֵ
                    float X = JsonObject->GetNumberField(TEXT("X"));
                    float Y = JsonObject->GetNumberField(TEXT("Y"));
                    float Z = JsonObject->GetNumberField(TEXT("Z"));

                    // ��ӵ��������
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
