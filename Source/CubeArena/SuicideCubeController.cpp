// Fill out your copyright notice in the Description page of Project Settings.


#include "SuicideCubeController.h"
#include "Engine/Engine.h"
#include "UserCube.h"
#include "SuicideCube.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Math/Vector.h"

ASuicideCubeController::ASuicideCubeController()
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

    SightConfig->SightRadius = AISightRadius;
    SightConfig->LoseSightRadius = AILoseSightRadius;
    SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
    SightConfig->SetMaxAge(AISightAge);

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ASuicideCubeController::OnPerceptionUpdated);
    GetPerceptionComponent()->ConfigureSense(*SightConfig);

    side = -1;
    time_togo_side = 50;

    toBeExecuted = NULL;


}



void ASuicideCubeController::BeginPlay()
{
    Super::BeginPlay();

    if (GetPerceptionComponent() != nullptr)
    {

//        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("AICube Controller Set"));
    }
    else
    {
//        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Some Problem Occured"));

    }

}



void ASuicideCubeController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if(toBeExecuted != NULL)
        toBeExecuted();

}



void ASuicideCubeController::getCloserToUser()
{

    if(user_cube != NULL)
    {
        ASuicideCube *suicide_cube = Cast<ASuicideCube>(GetPawn());

        if(suicide_cube != NULL)
        {

            const float &target_distance = suicide_cube->calculateDistanceTo(user_cube);

            const FVector &user_location = user_cube->GetActorLocation();
            const FVector &suicide_location = GetPawn()->GetActorLocation();
            const FRotator &suicide_rotation = UKismetMathLibrary::FindLookAtRotation(suicide_location, user_location);

            time_togo_side--;

            suicide_cube->turnThatRotation(suicide_rotation);
            suicide_cube->moveForward(FORWARD);
            suicide_cube->moveRight(side);

            if(target_distance <= 500)
            {
                time_togo_side = 1;
            }

            if(target_distance <= 150)
            {
                suicide_cube->killTheCubeWithDamage(suicide_cube->GetActorLocation());
            }

            if(time_togo_side <= 0)
            {
                suicide_cube->jump();

                side = (side) * (-1);

                time_togo_side = FMath::RandRange(30, 100);
            }
        }
    }
}



void ASuicideCubeController::OnPossess(APawn *Pawn)
{
    Super::OnPossess(Pawn);
}



void ASuicideCubeController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{

    int ret;

    int num = UpdatedActors.Num();

    for(int i=0; i < num;  i++)
    {

        AUserCube *user = Cast<AUserCube>(UpdatedActors[i]);

        if(user != NULL & enemy_detected == false)
        {
            enemy_detected = true;

            toBeExecuted = std::bind(&ASuicideCubeController::getCloserToUser, this);
            user_cube = user;

        }

    }
}

