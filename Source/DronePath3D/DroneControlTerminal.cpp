// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneControlTerminal.h"


void UDroneControlTerminal::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!UpdatedComponent || TargetLocation.Num() == 0)
    {
        return;
    }

    // ��ȡ��ǰ��λ��
    FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
    //   UE_LOG(LogTemp, Log, TEXT("Current location: %s"), *CurrentLocation.ToString());

       // ������ǻ�û�е���Ŀ��λ�ã���������
    if (point < TargetLocation.Num()) // ���Ŀ��㻹û������
    {
        FVector DesiredDirection = TargetLocation[point] - CurrentLocation; // ���㵱ǰλ����Ŀ���֮��ķ���
        DesiredDirection.Normalize(); // ��λ����������

        // ���㵱ǰλ����Ŀ��λ��֮��ľ���
        float DistanceToTarget = FVector::Dist(CurrentLocation, TargetLocation[point]);

        // ����������һ����λ��Ŀ�껹δ�������������
        if (DistanceToTarget > 10) // KINDA_SMALL_NUMBER ��һ����С��ֵ�������жϸ������
        {
            // ������з��򣨹�һ���ĵ�λ����������
            FVector MoveDirection = DesiredDirection.GetSafeNormal();

            // �����ٶȺ�DeltaTime����ÿ�η��еľ���
            FVector MoveStep = MoveDirection * Speed * DeltaTime;

            // ����λ��
            FVector NewLocation = CurrentLocation + MoveStep;

            // �ƶ�����λ��
            UpdatedComponent->MoveComponent(NewLocation - CurrentLocation, UpdatedComponent->GetComponentRotation(), true);
        }
        else
        {
            // ������˻��Ѿ�����Ŀ��λ�ã��л�����һ��Ŀ���
           // UE_LOG(LogTemp, Log, TEXT("arrived %d, flying to next point"), point);
            point++;
        }
    }
    else
    {
        // ������˻��ѵ������һ��Ŀ��λ�ã�ֹͣ�ƶ�
       // UE_LOG(LogTemp, Log, TEXT("stop move"));
    }
}
