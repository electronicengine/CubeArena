// Fill out your copyright notice in the Description page of Project Settings.


#include "UserCube.h"
#include "CubeLevelScript.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Bullet.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DestructibleComponent.h"


AUserCube::AUserCube() : solid_color(FMath::RandRange(0.0f, 0.3f),FMath::RandRange(0.0f, 0.2f), FMath::RandRange(0.0f, 0.2))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    spring_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    spring_arm->SetupAttachment(RootComponent);

    camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    camera->SetupAttachment(spring_arm);

    destructable_component = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructableComponent"));
    destructable_component->SetupAttachment(RootComponent);


 ;
    base_turn_rate = 45.0f;
    look_up_rate = 45.0f;

    trace_distance = 2000.0f;

}

// Called when the game starts or when spawned
void AUserCube::BeginPlay()
{
	Super::BeginPlay();

    level_script = Cast<ACubeLevelScript>(GetWorld()->GetLevelScriptActor());

    level_script->attachUserToLevelScript(this);

    destructable_component->CreateDynamicMaterialInstance(0);
    destructable_component->SetVectorParameterValueOnMaterials(FName("Color"), solid_color);
	
}

// Called every frame
void AUserCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUserCube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUserCube::fireBullet);


    PlayerInputComponent->BindAxis("MoveForward", this, &AUserCube::moveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AUserCube::moveRight);

    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AUserCube::baseTurnRate);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AUserCube::lookUpRate);

}



void AUserCube::setSolidColor(const FVector &Color)
{
    solid_color = Color;

    destructable_component->SetVectorParameterValueOnMaterials(FName("Color"), solid_color);

}



void AUserCube::moveForward(float Value)
{
    if((Controller) && Value != 0.0f)
    {
        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X);


        AddMovementInput(direction, Value);


    }
}



void AUserCube::moveRight(float Value)
{
    if((Controller) && Value != 0.0f)
    {
        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y);

        AddMovementInput(direction, Value);
    }
}



void AUserCube::lookUpRate(float Value)
{
    AddControllerPitchInput((Value * look_up_rate * (float)(GetWorld()->GetDeltaSeconds())));

}



void AUserCube::baseTurnRate(float Value)
{
    AddControllerYawInput(Value * base_turn_rate * (float)(GetWorld()->GetDeltaSeconds()));
}



void AUserCube::fireBullet()
{

    ABullet *bullet_ptr;
    if (bullet_container != NULL)
    {

        UWorld* const World = GetWorld();
        if (World != NULL)
        {

            FRotator spawn_rotation;
            FVector spawn_location;


            GetController()->GetPlayerViewPoint(spawn_location, spawn_rotation);
//            spawn_rotation= GetActorRotation();
            spawn_location = GetActorLocation() + (spawn_rotation.Vector() * 20) ;

            bullet_ptr = World->SpawnActor<ABullet>(bullet_container,  spawn_location, spawn_rotation);
//
            bullet_ptr->setSolidColor(solid_color);
            bullet_ptr->setBulletOwner(this);

        }
    }
}

