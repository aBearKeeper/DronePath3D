// Fill out your copyright notice in the Description page of Project Settings.
#include "UAStarPathPlanningAlgorithm.h"
#include "LidarPointCloud.h"


bool UUAStarPathPlanningAlgorithm::HaveCloudPoints(FVector Position)
{
    TArray<FLidarPointCloudPoint> SelectedPoints;

    FVector Extent = FVector(Unit_Distance, Unit_Distance, Unit_Distance);
    const bool bVisibleOnly = true;

    ULidarPointCloudBlueprintLibrary::GetPointsInBoxAsCopies(WorldContextObject, SelectedPoints, Position, Extent, bVisibleOnly);
    if (SelectedPoints.Num() > 0)
        return true;

    return false;

}

TArray<FVector> UUAStarPathPlanningAlgorithm::ExecuteAlgorithm(FVector StartPoint, FVector TargetPoint)
{
	// Implement A* algorithm here
    TArray<FNode*> FreshPoints;//δ������Ľڵ�
    TArray<FNode*> OldPoints;//�ѵ�����Ľڵ�

	TArray<FVector> RoutePoints;//����·��

    //��ʼ���ڵ�
    FNode* StartNode = new FNode(StartPoint);
    FNode* TargetNode = new FNode(TargetPoint);


    FreshPoints.Add(StartNode);

    int MaxIterations = 50000;  // ����������
    int Iterations = 0;

    while (FreshPoints.Num() > 0 && Iterations < MaxIterations)
    {
        //ѡ��δ����ڵ���acc��С��
        FNode* CurrentNode = *FreshPoints.GetData();
        for (FNode* node : FreshPoints)
        {
            if (node->Acc_Cost < CurrentNode->Acc_Cost)
            {
                CurrentNode = node;
            }
        }

        //�����Ŀ�������ݴ�������·��
        if (CurrentNode->Position.Equals(TargetPoint, 100.0f))
        {
            //    UE_LOG(LogTemp, Log, TEXT("arrived at: %s successful"), *CurrentNode->Position.ToString());
            while (CurrentNode != nullptr)
            {
                RoutePoints.Insert(CurrentNode->Position, 0);
                CurrentNode = CurrentNode->Parent;
            }
            break;
        }

        FreshPoints.Remove(CurrentNode);
        OldPoints.Add(CurrentNode);

        // UE_LOG(LogTemp, Log, TEXT("Unit_Distance : %d "), Unit_Distance);
         //�����ھ�����
        TArray <FVector> Neighbors = {

            FVector(CurrentNode->Position.X + Unit_Distance, CurrentNode->Position.Y, CurrentNode->Position.Z),
            FVector(CurrentNode->Position.X - Unit_Distance, CurrentNode->Position.Y, CurrentNode->Position.Z),
            FVector(CurrentNode->Position.X, CurrentNode->Position.Y + Unit_Distance, CurrentNode->Position.Z),
            FVector(CurrentNode->Position.X, CurrentNode->Position.Y - Unit_Distance, CurrentNode->Position.Z),
            FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z + Unit_Distance),
            FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z - Unit_Distance)

        };

        //�ж��ھ��Ƿ�ɴ�
        for (FVector neighbor : Neighbors)
        {
            bool isValid = 0;
            //���ɴ�������
            if (HaveCloudPoints(neighbor))
            {
                //  UE_LOG(LogTemp, Log, TEXT("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Can't Arrived!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %s"), *FVector(neighbor).ToString());
                continue;
            }


            //�����Ҳ����
            for (FNode* node : OldPoints)
            {
                if (node->Position.Equals(neighbor))
                {
                    isValid = 1;
                    break;
                }
            }
            if (isValid)
                continue;

            //�½��ڵ㲢����������
            FNode* NewNeighbor = new FNode(neighbor);
            NewNeighbor->F_Cost = CurrentNode->F_Cost + FVector::Dist(CurrentNode->Position, neighbor);
            NewNeighbor->G_Cost = FVector::Dist(neighbor, TargetPoint);
            NewNeighbor->Acc_Cost = NewNeighbor->F_Cost + NewNeighbor->G_Cost;
            NewNeighbor->Parent = CurrentNode;

            //����ھӽڵ��Ƿ���δ�����ڵ��в��Ҵ��۽�С
            isValid = 0;
            for (FNode* node : FreshPoints)
            {
                if (node->Position.Equals(NewNeighbor->Position) && node->Acc_Cost >= NewNeighbor->Acc_Cost)
                {
                    isValid = 1;
                    break;
                }
            }
            if (!isValid)
            {
                FreshPoints.Add(NewNeighbor);
                //   UE_LOG(LogTemp, Log, TEXT("FreshPoints.Add(NewNeighbor): %s"), *NewNeighbor->Position.ToString());
                //   UE_LOG(LogTemp, Log, TEXT("count: %d"), Iterations);
            }


        }
        Iterations++;
    }
    return RoutePoints;
}
