// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnWithCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APawnWithCamera::APawnWithCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    //Create our components
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    StaticMeshComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

    //Attach our components
    StaticMeshComp->SetupAttachment(RootComponent);
    SpringArmComp->SetupAttachment(StaticMeshComp);
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

    //Assign SpringArm class variables.
    SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
    SpringArmComp->TargetArmLength = 400.f;
    SpringArmComp->bEnableCameraLag = true;
    SpringArmComp->CameraLagSpeed = 3.0f;

    AutoPossessPlayer = EAutoReceiveInput::Player0;

    MaxFieldOfView = 90;

    MinFieldOfView = 60;

    MaxTargetArmLength = 400;

    MinTargetArmLength = 300;

    MaxPitch = -15;

    MinPitch = -80;
}

// Called when the game starts or when spawned
void APawnWithCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnWithCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    //��������˷Ŵ�ť��Ŵ󣬷������С
    {
        if (bZoomingIn)
        {
            ZoomFactor += DeltaTime / 0.5f;			//Zoom in over half a second
        }
        else
        {
            ZoomFactor -= DeltaTime / 0.25f;		//Zoom out over a quarter of a second
        }
        ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);

        //����ZoomFactor��������������ӳ��͵��ɱ۵ĳ���
        CameraComp->FieldOfView = FMath::Lerp<float>(MaxFieldOfView, MinFieldOfView, ZoomFactor);
        SpringArmComp->TargetArmLength = FMath::Lerp<float>(MaxTargetArmLength, MinTargetArmLength, ZoomFactor);
    }

    //��תActor��ƫת�Ƕȣ����������Ҳ����ת����Ϊ�������Actor�໥��
    {
        FRotator NewRotation = GetActorRotation();
        NewRotation.Yaw += CameraInput.X;
        SetActorRotation(NewRotation);
    }

    // ��ת������ĸ����Ƕȣ���Ҫ����������ƣ��������Ǿ���ʼ�ո���Actor
    {
        FRotator NewRotation = SpringArmComp->GetComponentRotation();
        NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, MinPitch, MaxPitch);
        SpringArmComp->SetWorldRotation(NewRotation);
    }

    // ����"MoveX"�� "MoveY"�������������ƶ�
    {
        if (!MovementInput.IsZero())
        {
            // ���ƶ����������ֵ�Ŵ�100��
            MovementInput = MovementInput.GetSafeNormal() * 1000.0f;
            FVector NewLocation = GetActorLocation();
            NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
            NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
            NewLocation += GetActorUpVector() * MovementInput.Z * DeltaTime;
            SetActorLocation(NewLocation);
        }
    }
}

// Called to bind functionality to input
void APawnWithCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // ��"ZoomIn"���¼�
    InputComponent->BindAction("ZoomIn", IE_Pressed, this, &APawnWithCamera::ZoomIn);
    InputComponent->BindAction("ZoomIn", IE_Released, this, &APawnWithCamera::ZoomOut);

    //Ϊ��������¼���ÿ֡���ã�
    InputComponent->BindAxis("MoveForward", this, &APawnWithCamera::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &APawnWithCamera::MoveRight);
    InputComponent->BindAxis("MoveUp", this, &APawnWithCamera::MoveUp);
    InputComponent->BindAxis("CameraPitch", this, &APawnWithCamera::PitchCamera);
    InputComponent->BindAxis("CameraYaw", this, &APawnWithCamera::YawCamera);



}

// ���뺯��
void APawnWithCamera::MoveForward(float AxisValue)
{
    MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APawnWithCamera::MoveRight(float AxisValue)
{
    MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APawnWithCamera::MoveUp(float AxisValue)
{
    MovementInput.Z = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APawnWithCamera::PitchCamera(float AxisValue)
{
    CameraInput.Y = AxisValue;
}

void APawnWithCamera::YawCamera(float AxisValue)
{
    CameraInput.X = AxisValue;
}

void APawnWithCamera::ZoomIn()
{
    bZoomingIn = true;
}

void APawnWithCamera::ZoomOut()
{
    bZoomingIn = false;
}

