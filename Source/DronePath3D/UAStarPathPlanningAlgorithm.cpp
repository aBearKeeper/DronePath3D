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
    TArray<FNode*> FreshPoints;//未到达过的节点
    TArray<FNode*> OldPoints;//已到达过的节点

	TArray<FVector> RoutePoints;//最终路径

    //初始化节点
    FNode* StartNode = new FNode(StartPoint);
    FNode* TargetNode = new FNode(TargetPoint);


    FreshPoints.Add(StartNode);

    int MaxIterations = 50000;  // 最大迭代次数
    int Iterations = 0;

    while (FreshPoints.Num() > 0 && Iterations < MaxIterations)
    {
        //选出未到达节点中acc最小的
        FNode* CurrentNode = *FreshPoints.GetData();
        for (FNode* node : FreshPoints)
        {
            if (node->Acc_Cost < CurrentNode->Acc_Cost)
            {
                CurrentNode = node;
            }
        }

        //如果是目标点则回溯储存完整路径
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
         //计算邻居坐标
        TArray <FVector> Neighbors = {

            FVector(CurrentNode->Position.X + Unit_Distance, CurrentNode->Position.Y, CurrentNode->Position.Z),
            FVector(CurrentNode->Position.X - Unit_Distance, CurrentNode->Position.Y, CurrentNode->Position.Z),
            FVector(CurrentNode->Position.X, CurrentNode->Position.Y + Unit_Distance, CurrentNode->Position.Z),
            FVector(CurrentNode->Position.X, CurrentNode->Position.Y - Unit_Distance, CurrentNode->Position.Z),
            FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z + Unit_Distance),
            FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z - Unit_Distance)

        };

        //判断邻居是否可达
        for (FVector neighbor : Neighbors)
        {
            bool isValid = 0;
            //不可达则跳过
            if (HaveCloudPoints(neighbor))
            {
                //  UE_LOG(LogTemp, Log, TEXT("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Can't Arrived!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %s"), *FVector(neighbor).ToString());
                continue;
            }


            //到达过也跳过
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

            //新建节点并计算其内容
            FNode* NewNeighbor = new FNode(neighbor);
            NewNeighbor->F_Cost = CurrentNode->F_Cost + FVector::Dist(CurrentNode->Position, neighbor);
            NewNeighbor->G_Cost = FVector::Dist(neighbor, TargetPoint);
            NewNeighbor->Acc_Cost = NewNeighbor->F_Cost + NewNeighbor->G_Cost;
            NewNeighbor->Parent = CurrentNode;

            //检查邻居节点是否在未经过节点中并且代价较小
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
