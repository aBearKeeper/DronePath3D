// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneControlTerminal.h"


void UDroneControlTerminal::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!UpdatedComponent || TargetLocation.Num() == 0)
    {
        return;
    }

    // 获取当前的位置
    FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
    //   UE_LOG(LogTemp, Log, TEXT("Current location: %s"), *CurrentLocation.ToString());

       // 如果我们还没有到达目标位置，继续飞行
    if (point < TargetLocation.Num()) // 如果目标点还没遍历完
    {
        FVector DesiredDirection = TargetLocation[point] - CurrentLocation; // 计算当前位置与目标点之间的方向
        DesiredDirection.Normalize(); // 单位化方向向量

        // 计算当前位置与目标位置之间的距离
        float DistanceToTarget = FVector::Dist(CurrentLocation, TargetLocation[point]);

        // 如果距离大于一个单位（目标还未到达），继续飞行
        if (DistanceToTarget > 10) // KINDA_SMALL_NUMBER 是一个很小的值，用来判断浮动误差
        {
            // 计算飞行方向（归一化的单位方向向量）
            FVector MoveDirection = DesiredDirection.GetSafeNormal();

            // 根据速度和DeltaTime计算每次飞行的距离
            FVector MoveStep = MoveDirection * Speed * DeltaTime;

            // 更新位置
            FVector NewLocation = CurrentLocation + MoveStep;

            // 移动到新位置
            UpdatedComponent->MoveComponent(NewLocation - CurrentLocation, UpdatedComponent->GetComponentRotation(), true);
        }
        else
        {
            // 如果无人机已经到达目标位置，切换到下一个目标点
           // UE_LOG(LogTemp, Log, TEXT("arrived %d, flying to next point"), point);
            point++;
        }
    }
    else
    {
        // 如果无人机已到达最后一个目标位置，停止移动
       // UE_LOG(LogTemp, Log, TEXT("stop move"));
    }
}
