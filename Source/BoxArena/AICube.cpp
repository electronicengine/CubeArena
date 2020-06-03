// Fill out your copyright notice in the Description page of Project Settings.


#include "AICube.h"
#include "CubeLevelScript.h"
#include "Bullet.h"
#include "DestructibleComponent.h"



// Sets default values
AAICube::AAICube()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


    destructable_component = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructableComponent"));
    destructable_component->SetupAttachment(RootComponent);

    dying_counter = 200;
    died = false;
    time_to_count = 100;
    toBeExecuted = NULL;
}



// Called when the game starts or when spawned
void AAICube::BeginPlay()
{
	Super::BeginPlay();

    level_script = Cast<ACubeLevelScript>(GetWorld()->GetLevelScriptActor());

    level_script->addList(this);

    destructable_component->CreateDynamicMaterialInstance(0);
    destructable_component->SetVectorParameterValueOnMaterials(FName("Color"), FVector(FMath::RandRange(0.0f, 0.2f),
                                                                                       FMath::RandRange(0.0f, 0.2f),
                                                                                       FMath::RandRange(0.0f, 0.3f)));



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
            fireBullet();
            time_to_count = 100;
        }

    }

    if(died == true)
    {
        dying_counter--;
        if(dying_counter == 0)
            Destroy();
    }


}



// Called to bind functionality to input
void AAICube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AAICube::killTheCube(const FVector &ImpactPoint)
{
    destructable_component->ApplyRadiusDamage(10.f, ImpactPoint, 10.0f, 10.0f, true);
    died = true;
//    Destroy();
}



void AAICube::assignAiId(int Id)
{

    cube_id = Id;

    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Cube Assigned ID: ") + FString::FromInt(cube_id));

}



int AAICube::getAiId()
{
    return cube_id;
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



void AAICube::fireBullet()
{
    if (bullet_container != NULL)
    {
        UWorld* const World = GetWorld();
        if (World != NULL)
        {

            FRotator spawn_rotation;
            FVector spawn_location;


            spawn_rotation= GetActorRotation();
            spawn_location = GetActorLocation() + (spawn_rotation.Vector() * 20) ;

            spawn_rotation.Pitch = spawn_rotation.Pitch + 1.5f;

            World->SpawnActor<ABullet>(bullet_container, spawn_location, spawn_rotation);


        }
    }
}


