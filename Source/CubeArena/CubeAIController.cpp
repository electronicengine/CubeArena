// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeAIController.h"
#include "Engine/Engine.h"
#include "UserCube.h"
#include "AICube.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Math/Vector.h"


ACubeAIController::ACubeAIController()
{
    PrimaryActorTick.bCanEverTick = true;

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
    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ACubeAIController::OnPerceptionUpdated);
    GetPerceptionComponent()->ConfigureSense(*SightConfig);

    side = -1;
    direction = 1;
    time_togo_forward = 30;
    time_togo_side = 50;

    toBeExecuted = NULL;

    min_dist_to_close = 600.0f;
    min_dist_to_fire = 3200.0f;
}



void ACubeAIController::keepDistanceFromUser()
{

    if(user_cube != NULL)
    {
        AAICube *ai_cube = Cast<AAICube>(GetPawn());
        if(ai_cube != NULL)
        {
            const float &target_distance = ai_cube->calculateDistanceTo(user_cube);

            const FVector &user_location = user_cube->GetActorLocation();
            const FVector &ai_location = GetPawn()->GetActorLocation();
            const FRotator &ai_rotation = UKismetMathLibrary::FindLookAtRotation(ai_location, user_location);

            if(target_distance <= min_dist_to_fire)
                  ai_cube->fire_free = true;
            else
                ai_cube->fire_free = false;

            if(target_distance <= distance_to_close)
            {

                distance_to_close = FMath::RandRange(min_dist_to_close, min_dist_to_fire);

                toBeExecuted = std::bind(&ACubeAIController::turnAround, this);

            }
        }

    }
}



void ACubeAIController::getCloserToUser()
{
    if(user_cube != NULL)
    {
        AAICube *ai_cube = Cast<AAICube>(GetPawn());
        if(ai_cube != NULL)
        {
            const float &target_distance = ai_cube->calculateDistanceTo(user_cube);

            const FVector &user_location = user_cube->GetActorLocation();
            const FVector &ai_location = GetPawn()->GetActorLocation();
            const FRotator &ai_rotation = UKismetMathLibrary::FindLookAtRotation(ai_location, user_location);


            if(target_distance <= min_dist_to_fire)
                  ai_cube->fire_free = true;
            else
                ai_cube->fire_free = false;

            if(target_distance <= distance_to_close)
            {

                distance_to_close = FMath::RandRange(min_dist_to_close, min_dist_to_fire);
                toBeExecuted = std::bind(&ACubeAIController::turnAround, this);
            }
            else
            {

                ai_cube->turnThatRotation(ai_rotation);
                ai_cube->moveForward(FORWARD);
            }
        }
    }
}



void ACubeAIController::turnAround()
{

    if(user_cube != NULL)
    {
        AAICube *ai_cube = Cast<AAICube>(GetPawn());
        if(ai_cube != NULL)
        {
            const float &target_distance = ai_cube->calculateDistanceTo(user_cube);


            const FVector &user_location = user_cube->GetActorLocation();
            const FVector &ai_location = GetPawn()->GetActorLocation();
            const FRotator &ai_rotation = UKismetMathLibrary::FindLookAtRotation(ai_location, user_location);



            if(target_distance >= min_dist_to_fire)
            {
                distance_to_close = FMath::RandRange(min_dist_to_close, min_dist_to_fire);
                toBeExecuted = std::bind(&ACubeAIController::getCloserToUser, this);
            }
            else if(target_distance <= min_dist_to_close)
            {

                time_togo_side--;

                ai_cube->turnThatRotation(ai_rotation);
                ai_cube->moveForward(BACKWARD);
                ai_cube->moveRight(side);

                if(time_togo_side <= 0)
                {
                    ai_cube->jump();

                    side = (side) * (-1);

                    time_togo_side = FMath::RandRange(30, 100);
                }
            }
            else
            {
                time_togo_forward--;
                time_togo_side--;

                ai_cube->turnThatRotation(ai_rotation);
                ai_cube->moveForward(direction);
                ai_cube->moveRight(side);

                if(time_togo_forward <= 0)
                {
                    ai_cube->jump();

                    direction = (direction) * (-1);

                    time_togo_forward = FMath::RandRange(30, 100);
                }

                if(time_togo_side <= 0)
                {
                    ai_cube->jump();

                    side = (side) * (-1);

                    time_togo_side = FMath::RandRange(30, 100);
                }
            }
        }
    }
}



void ACubeAIController::OnPossess(APawn *Pawn)
{
    Super::OnPossess(Pawn);
}



void ACubeAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if(toBeExecuted != NULL)
        toBeExecuted();

}



void ACubeAIController::BeginPlay()
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



void ACubeAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{

    int ret;

    int num = UpdatedActors.Num();

    for(int i=0; i < num;  i++)
    {

        AUserCube *user = Cast<AUserCube>(UpdatedActors[i]);

        if(user != NULL & enemy_detected == false)
        {
            enemy_detected = true;


            distance_to_close = FMath::RandRange(min_dist_to_close, min_dist_to_fire);
            toBeExecuted = std::bind(&ACubeAIController::getCloserToUser, this);
            user_cube = user;

        }

    }
}



