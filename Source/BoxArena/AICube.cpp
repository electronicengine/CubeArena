// Fill out your copyright notice in the Description page of Project Settings.


#include "AICube.h"
#include "CubeLevelScript.h"
#include "Bullet.h"
#include "DestructibleComponent.h"



// Sets default values
AAICube::AAICube() : solid_color(FMath::RandRange(0.0f, 0.3f),FMath::RandRange(0.0f, 0.2f), FMath::RandRange(0.0f, 0.2))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


    destructable_component = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructableComponent"));
    destructable_component->SetupAttachment(RootComponent);

    dying_counter = 200;
    died = false;
    time_to_count = 200;
    toBeExecuted = NULL;
}



// Called when the game starts or when spawned
void AAICube::BeginPlay()
{
	Super::BeginPlay();

    level_script = Cast<ACubeLevelScript>(GetWorld()->GetLevelScriptActor());

    destructable_component->CreateDynamicMaterialInstance(0);
    destructable_component->SetVectorParameterValueOnMaterials(FName("Color"), solid_color);



}



// Called every frame
void AAICube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    time_to_count--;

    if(time_to_count == 0)
    {
        if(died != true)
        {
            if(target_distance <= 3500)
                fireBullet();
            time_to_count = 200;
        }

    }

    if(died == true)
    {
//        dying_counter--;
//        if(dying_counter == 0)
//            Destroy();
    }

    InitialLifeSpan = 0;
    SetLifeSpan(0);

}



// Called to bind functionality to input
void AAICube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AAICube::killTheCube(const FVector &ImpactPoint)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Cube Killing.."));
    SetLifeSpan(40);
    destructable_component->ApplyRadiusDamage(15.f, ImpactPoint, 5.0f, 5.0f, true);
    died = true;
}



void AAICube::assignAiId(int Id)
{

    cube_id = Id;

//    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Cube Assigned ID: ") + FString::FromInt(cube_id));

}



int AAICube::getAiId()
{
    return cube_id;
}

FVector &AAICube::getSolidColor()
{
    return solid_color;
}



void AAICube::moveForward(float Value)
{
    if((Controller) && Value != 0.0f)
    {
        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X);


        AddMovementInput(direction, Value);


    }
}



void AAICube::moveRight(float Value)
{
    if((Controller) && Value != 0.0f)
    {
        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y);

        AddMovementInput(direction, Value);
    }
}



void AAICube::turn(FRotator &Rotation)
{
//    AddControllerPitchInput((Value * look_up_rate * (float)(GetWorld()->GetDeltaSeconds())));

}



void AAICube:: setTargetDistance(float Distance)
{
    target_distance = Distance;


}



void AAICube::fireBullet()
{

    ABullet *bullet;

    if (bullet_container != NULL)
    {
        UWorld* const World = GetWorld();
        if (World != NULL)
        {

            FRotator spawn_rotation;
            FVector spawn_location;


            spawn_rotation= GetActorRotation();
            spawn_location = GetActorLocation() + (spawn_rotation.Vector() * 30) ;

            if(target_distance >= 3200)
                spawn_rotation.Pitch = spawn_rotation.Pitch + 2.5f;
            else if((target_distance >= 5000))
                spawn_rotation.Pitch = spawn_rotation.Pitch + 5.0f;
            else if((target_distance >= 7000))
                spawn_rotation.Pitch = spawn_rotation.Pitch + 6.0f;
            else
                spawn_rotation.Pitch = spawn_rotation.Pitch + 1.5f;

//            spawn_rotation.GetDenormalized();

            bullet = World->SpawnActor<ABullet>(bullet_container, spawn_location, spawn_rotation);
            bullet->setSolidColor(solid_color);
            bullet->setBulletOwner(this);

        }
    }
}


