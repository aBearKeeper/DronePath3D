// Fill out your copyright notice in the Description page of Project Settings.


#include "PotentialFieldAlgorithm.h"
#include "LidarPointCloud.h"


bool UPotentialFieldAlgorithm::HaveCloudPoints(FVector Position, UObject* WorldContextObject)
{
	TArray<FLidarPointCloudPoint> SelectedPoints;

	double DRatio;
	if (Parameters.Contains("DRatio")) {
		DRatio = Parameters["DRatio"];
	}
	else {
		DRatio = 1.2;
	}

	double UnitDistance;
	if (Parameters.Contains("Diameter")) {
		UnitDistance = Parameters["Diameter"] * 150 * DRatio;
	}
	else {
		UnitDistance = 1 * 150 * DRatio;
	}

	FVector Extent = FVector(UnitDistance, UnitDistance, UnitDistance);
	const bool bVisibleOnly = true;

	ULidarPointCloudBlueprintLibrary::GetPointsInBoxAsCopies(WorldContextObject, SelectedPoints, Position, Extent, bVisibleOnly);

	if (SelectedPoints.Num() > 0)
		return true;

	return false;

}

TArray<FVector> UPotentialFieldAlgorithm::ExecuteAlgorithm(FVector StartPoint, FVector TargetPoint, UObject* WorldContextObject)
{

	double DRatio;
	if (Parameters.Contains("DRatio")) {
		DRatio = Parameters["DRatio"];
	}
	else {
		DRatio = 1.2;
	}

	double MoveDistance;
	if (Parameters.Contains("Diameter")) {
		MoveDistance = Parameters["Diameter"] * 200 * DRatio;
	}
	else {
		MoveDistance = 1 * 200 * DRatio;
	}

	int NeighborHighPrecision;
	if (Parameters.Contains("NeighborHighPrecision")) {
		double DNeighborHighPrecision = Parameters["NeighborHighPrecision"];
		NeighborHighPrecision = int(DNeighborHighPrecision);
	}
	else {
		NeighborHighPrecision = 0;
	}

	// Implement A* algorithm here
	TArray<FNode*> FreshPoints;//未到达过的节点
	TArray<FNode*> OldPoints;//已到达过的节点

	TArray<FVector> RoutePoints;//最终路径

	//初始化节点
	FNode* StartNode = new FNode(StartPoint);
	FNode* TargetNode = new FNode(TargetPoint);


	FreshPoints.Add(StartNode);

	// int MaxIterations = 50000;  // 最大迭代次数
	int Iterations = 0;

	while (FreshPoints.Num() > 0)
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
		if (CurrentNode->Position.Equals(TargetPoint, MoveDistance / 2))
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

		TArray <FVector> Neighbors = {};
		switch (NeighborHighPrecision) {
		case 0:
			Neighbors = {
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z),
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z),
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z),
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z),
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance),
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance)
			};
			break;
		case 1:
			Neighbors = {
				// 六个基本方向
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z), // 前
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z), // 后
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z), // 右
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z), // 左
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance), // 上
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance), // 下

				// 平面对角线方向（XY平面）
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z), // 前右
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z), // 前左
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z), // 后右
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z), // 后左

				// 平面对角线方向（XZ平面）
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance), // 前上
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance), // 前下
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance), // 后上
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance), // 后下

				// 平面对角线方向（YZ平面）
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z + MoveDistance), // 右上
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z - MoveDistance), // 右下
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z + MoveDistance), // 左上
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z - MoveDistance), // 左下
			};
			break;
		case 2:
			Neighbors = {
				// 六个基本方向
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z), // 前
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z), // 后
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z), // 右
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z), // 左
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance), // 上
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance), // 下

				// 平面对角线方向（XY平面）
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z), // 前右
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z), // 前左
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z), // 后右
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z), // 后左

				// 平面对角线方向（XZ平面）
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance), // 前上
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance), // 前下
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance), // 后上
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance), // 后下

				// 平面对角线方向（YZ平面）
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z + MoveDistance), // 右上
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z - MoveDistance), // 右下
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z + MoveDistance), // 左上
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z - MoveDistance), // 左下

				// 三维对角线方向
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z + MoveDistance), // 前右上
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z - MoveDistance), // 前右下
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z + MoveDistance), // 前左上
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z - MoveDistance), // 前左下
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z + MoveDistance), // 后右上
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z - MoveDistance), // 后右下
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z + MoveDistance), // 后左上
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z - MoveDistance)  // 后左下
			};
			break;
		default:
			Neighbors = {
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z),
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z),
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z),
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z),
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance),
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance)
			};
		}

		//判断邻居是否可达
		for (FVector neighbor : Neighbors)
		{
			bool isValid = 0;
			//不可达则跳过
			if (HaveCloudPoints(neighbor, WorldContextObject))
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
			NewNeighbor->Acc_Cost = NewNeighbor->G_Cost;
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

