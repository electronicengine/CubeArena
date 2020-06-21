// Fill out your copyright notice in the Description page of Project Settings.


#include "AICube.h"
#include "Components/StaticMeshComponent.h"
#include "CubeLevelActor.h"
#include "Bullet.h"
#include "TimerManager.h"
#include "DestructibleCube.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "CubeGameInstance.h"


AAICube::AAICube() : solid_color(FMath::RandRange(0.0f, 0.2f),FMath::RandRange(0.0f, 0.15f), FMath::RandRange(0.0f, 0.2))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    ai_solid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AISolid"));
    ai_solid->SetupAttachment(RootComponent);

    ai_face = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AIFace"));
    ai_face->SetupAttachment(RootComponent);


    died = false;
    fire_free = false;
    fire_accurancy = 20.0f;

}



// Called when the game starts or when spawned
void AAICube::BeginPlay()
{
    Super::BeginPlay();

    level_script = Cast<ACubeLevelActor>(GetWorld()->GetLevelScriptActor());

    cube_id = level_script->takeId(this);

    ai_solid->CreateDynamicMaterialInstance(0);
    ai_solid->SetVectorParameterValueOnMaterials(FName("Color"), solid_color);

    ai_face->CreateDynamicMaterialInstance(0);
    ai_face->SetVectorParameterValueOnMaterials(FName("Color"), FVector(FMath::RandRange(0.0f, 0.2f),
                                                                        FMath::RandRange(0.0f, 0.2f),
                                                                        FMath::RandRange(0.0f, 0.2f)));

    selectAFace();


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



// Sets default values
void AAICube::selectAFace()
{
    UCubeGameInstance * game_instance = Cast<UCubeGameInstance>(GetGameInstance());

    ai_face->SetVectorParameterValueOnMaterials(FName("FaceParam"), game_instance->getface(FMath::RandRange(0, 24)));
}


void AAICube::killTheCube(const FVector &ImpactPoint)
{

    FVector player_loc ;
    FRotator rotation;
    ADestructibleCube *destructible;

    if(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) != NULL)
    {
        player_loc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
        rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player_loc);
    }
    else
        rotation = FRotator(0,0,0);

    destructible = GetWorld()->SpawnActor<ADestructibleCube>(destructible_container, GetActorLocation(), rotation);
    destructible->setSolidColor(solid_color);

    Destroy();
    destructible->expolode(ImpactPoint, 1000.0f);

    died = true;

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
            spawn_location = GetActorLocation() + (spawn_rotation.Vector() * 90) ;


            spawn_rotation.Pitch = spawn_rotation.Pitch + FIRE_OFSETT_PITCH;
            spawn_rotation.Yaw = spawn_rotation.Yaw + FMath::RandRange(-fire_accurancy, fire_accurancy);

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
