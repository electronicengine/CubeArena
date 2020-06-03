// Fill out your copyright notice in the Description page of Project Settings.


#include "AICubeController.h"
#include "Engine/Engine.h"
#include "UserCube.h"
#include "AICube.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Math/Vector.h"


AAICubeController::AAICubeController()
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
    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAICubeController::OnPerceptionUpdated);
    GetPerceptionComponent()->ConfigureSense(*SightConfig);

    side = -1;
    direction = 1;
    time_to_count = 0;
    toBeExecuted = NULL;
}



void AAICubeController::getCloserToUser(AUserCube *User)
{
    int ret;

    if(Cast<AAICube>(GetPawn())->died != true)
    {

        ret = MoveToActor(user_cube, FMath::RandRange(600.0f, 3000.0f));

        if(ret != EPathFollowingRequestResult::RequestSuccessful)
        {
            time_to_count = FMath::RandRange(30, 100);
            toBeExecuted = std::bind(&AAICubeController::turnAroundRight, this, User);
        }
    }


}





void AAICubeController::fireToUser(AUserCube *User)
{

}



void AAICubeController::turnAroundRight(AUserCube *User)
{


    if(Cast<AAICube>(GetPawn())->died != true)
    {
        time_to_count--;


        if(GetPawn()->GetDistanceTo(User) >= 3500)
            toBeExecuted = std::bind(&AAICubeController::getCloserToUser, this, User);

        const FVector &user_location = User->GetActorLocation();
        const FVector &ai_location = GetPawn()->GetActorLocation();

        GetPawn()->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(ai_location, user_location));

        Cast<AAICube>(GetPawn())->moveRight(direction);
        Cast<AAICube>(GetPawn())->moveForward(side);

        if(time_to_count == 0)
        {

            Cast<AAICube>(GetPawn())->Jump();
            direction = -1*direction;
            side = -1*side;

            time_to_count =  FMath::RandRange(30, 100);
        }
    }

}



void AAICubeController::turnAroundLeft(AUserCube *User)
{

}



void AAICubeController::OnPossess(APawn *Pawn)
{
    Super::OnPossess(Pawn);
}



void AAICubeController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if(user_cube != NULL)
        Cast<AAICube>(GetPawn())->setTargetDistance(GetPawn()->GetDistanceTo(user_cube));

    if(toBeExecuted != NULL)
        toBeExecuted(user_cube);



}



void AAICubeController::BeginPlay()
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



void AAICubeController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{

    int ret;

    int num = UpdatedActors.Num();


    for(int i=0; i < num;  i++)
    {

        AUserCube *user = Cast<AUserCube>(UpdatedActors[i]);

        if(user != NULL & enemy_detected == false)
        {
            enemy_detected = true;
            AAICube *ai_cube = Cast<AAICube>(GetPawn());

//            ai_cube->moveRight(1);
//            FRotation
//            toBeExecuted = std::bind(&AAICubeController::getCloserToUser, this, user);
            user_cube = user;

        }

    }
}
