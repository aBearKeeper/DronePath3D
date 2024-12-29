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
	TArray<FNode*> FreshPoints;//δ������Ľڵ�
	TArray<FNode*> OldPoints;//�ѵ�����Ľڵ�

	TArray<FVector> RoutePoints;//����·��

	//��ʼ���ڵ�
	FNode* StartNode = new FNode(StartPoint);
	FNode* TargetNode = new FNode(TargetPoint);


	FreshPoints.Add(StartNode);

	// int MaxIterations = 50000;  // ����������
	int Iterations = 0;

	while (FreshPoints.Num() > 0)
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
		 //�����ھ�����

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
				// ������������
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z), // ǰ
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z), // ��
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z), // ��
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z), // ��
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance), // ��
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance), // ��

				// ƽ��Խ��߷���XYƽ�棩
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z), // ǰ��
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z), // ǰ��
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z), // ����
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z), // ����

				// ƽ��Խ��߷���XZƽ�棩
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance), // ǰ��
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance), // ǰ��
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance), // ����
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance), // ����

				// ƽ��Խ��߷���YZƽ�棩
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z + MoveDistance), // ����
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z - MoveDistance), // ����
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z + MoveDistance), // ����
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z - MoveDistance), // ����
			};
			break;
		case 2:
			Neighbors = {
				// ������������
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z), // ǰ
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z), // ��
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z), // ��
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z), // ��
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance), // ��
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance), // ��

				// ƽ��Խ��߷���XYƽ�棩
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z), // ǰ��
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z), // ǰ��
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z), // ����
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z), // ����

				// ƽ��Խ��߷���XZƽ�棩
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance), // ǰ��
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance), // ǰ��
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z + MoveDistance), // ����
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y, CurrentNode->Position.Z - MoveDistance), // ����

				// ƽ��Խ��߷���YZƽ�棩
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z + MoveDistance), // ����
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z - MoveDistance), // ����
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z + MoveDistance), // ����
				FVector(CurrentNode->Position.X, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z - MoveDistance), // ����

				// ��ά�Խ��߷���
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z + MoveDistance), // ǰ����
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z - MoveDistance), // ǰ����
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z + MoveDistance), // ǰ����
				FVector(CurrentNode->Position.X + MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z - MoveDistance), // ǰ����
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z + MoveDistance), // ������
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y + MoveDistance, CurrentNode->Position.Z - MoveDistance), // ������
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z + MoveDistance), // ������
				FVector(CurrentNode->Position.X - MoveDistance, CurrentNode->Position.Y - MoveDistance, CurrentNode->Position.Z - MoveDistance)  // ������
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

		//�ж��ھ��Ƿ�ɴ�
		for (FVector neighbor : Neighbors)
		{
			bool isValid = 0;
			//���ɴ�������
			if (HaveCloudPoints(neighbor, WorldContextObject))
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
			NewNeighbor->Acc_Cost = NewNeighbor->G_Cost;
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

