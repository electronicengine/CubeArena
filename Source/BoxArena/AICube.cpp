// Fill out your copyright notice in the Description page of Project Settings.


#include "AICube.h"
#include "CubeLevelScript.h"
#include "Bullet.h"
#include "DestructibleComponent.h"
#include "TimerManager.h"


// Sets default values
AAICube::AAICube() : solid_color(FMath::RandRange(0.0f, 0.3f),FMath::RandRange(0.0f, 0.2f), FMath::RandRange(0.0f, 0.2))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


    destructable_component = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructableComponent"));
    destructable_component->SetupAttachment(RootComponent);


    died = false;
    fire_free = false;
}



// Called when the game starts or when spawned
void AAICube::BeginPlay()
{
	Super::BeginPlay();

    level_script = Cast<ACubeLevelScript>(GetWorld()->GetLevelScriptActor());

    cube_id = level_script->takeId(this);

    destructable_component->CreateDynamicMaterialInstance(0);
    destructable_component->SetVectorParameterValueOnMaterials(FName("Color"), solid_color);

    GetWorld()->GetTimerManager().ClearTimer(timer_handle_destroy);

}



// Called every frame
void AAICube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



// Called to bind functionality to input
void AAICube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AAICube::killTheCube(const FVector &ImpactPoint)
{
//    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Cube Killing.."));
    destructable_component->ApplyRadiusDamage(20.f, GetActorLocation(), 20.0f, 20.0f, true);
    died = true;
    GetWorld()->GetTimerManager().SetTimer(timer_handle_destroy, this, &AAICube::eraseAI, 2.0f, false);

}



void AAICube::assignAiId(int Id)
{

    cube_id = Id;

//    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Cube Assigned ID: ") + FString::FromInt(cube_id));

}



void AAICube::moveForward(float Value)
{
    if((Controller) && Value != 0.0f && died != true)
    {
        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X);

        AddMovementInput(direction, Value);
    }
}



void AAICube::moveRight(float Value)
{
    if((Controller) && Value != 0.0f && died != true)
    {
        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y);

        AddMovementInput(direction, Value);
    }
}



void AAICube::fireBullet()
{

    ABullet *bullet;

    if (bullet_container != NULL && died != true && fire_free == true)
    {
        UWorld* const World = GetWorld();
        if (World != NULL)
        {

            FRotator spawn_rotation;
            FVector spawn_location;


            spawn_rotation= GetActorRotation();
            spawn_location = GetActorLocation() + (spawn_rotation.Vector() * 30) ;


            spawn_rotation.Pitch = spawn_rotation.Pitch + FIRE_OFSETT_PITCH;

            bullet = World->SpawnActor<ABullet>(bullet_container, spawn_location, spawn_rotation);
            bullet->setSolidColor(solid_color);
            bullet->setBulletOwner(this);

        }
    }
}



void AAICube::setFireRate(const float &FireRate)
{


    GetWorld()->GetTimerManager().ClearTimer(timer_handle_fire);
    GetWorld()->GetTimerManager().SetTimer(timer_handle_fire, this, &AAICube::fireBullet, FireRate + FMath::RandRange(0.0f, 0.5f), true);
}



float AAICube::calculateDistanceTo(AActor *Actor)
{
    return GetDistanceTo(Actor);
}



void AAICube::eraseAI()
{
    Destroy();
}



int AAICube::getAiId()
{
    int value = cube_id;

    return value;
}



FVector &AAICube::getSolidColor()
{
    return solid_color;
}



void AAICube::jump()
{
    if(died != true)
        Jump();
}



void AAICube::turnThatRotation(const FRotator &Rotation)
{
    SetActorRotation(Rotation);
}
