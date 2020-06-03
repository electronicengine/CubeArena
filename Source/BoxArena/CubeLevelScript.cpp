// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeLevelScript.h"
#include "Engine/Engine.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"



ACubeLevelScript::ACubeLevelScript() : arena_size{8000.0f, 8000.0f, 55.0f}
{

    PrimaryActorTick.bCanEverTick = true;

    difficulty = 2;
    game_current_level = 1;
    time_to_count = 0;
    user_point = 0;


}



void ACubeLevelScript::Tick(float DeltaTime)
{

    Super::Tick(DeltaTime);

    if(time_to_count <= 0)
    {
        time_to_count = 0;

        if(toBeExecuted != NULL)
            toBeExecuted();
    }
    else
    {
        time_to_count--;
    }

}



void ACubeLevelScript::BeginPlay()
{

    Super::BeginPlay();
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Level-script: spawn"));

    toBeExecuted = std::bind(&ACubeLevelScript::spawnAI, this);

}



void ACubeLevelScript::attachUserToLevelScript(AUserCube *UserCube)
{

    user_cube = UserCube;
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("attachUserToLevelScript"));

}



void ACubeLevelScript::aiBulletHitCallBack()
{

}



void ACubeLevelScript::userBulletHitCallBack(AActor *HitActor, const FVector ImpactPoint)
{


    AAICube *cube = Cast<AAICube>(HitActor);

    int cube_id = cube->getAiId();
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Enemy Kill Point - AI ID: ") + FString::FromInt(cube_id));


    if(cube != NULL)
    {
        std::list<AAICube *>::iterator it;

        for(it = ai_list.begin(); it != ai_list.end(); it++)
        {
            if(cube_id == (*it)->getAiId())
                ai_list.erase(it++);

        }

        user_point++;
        cube->killTheCube(ImpactPoint);

    }


    if(ai_list.size() == 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Next Level "));
        game_current_level++;

        toBeExecuted = std::bind(&ACubeLevelScript::spawnAI, this);
    }

}

void ACubeLevelScript::addList(AAICube *AICube)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Addlist "));

    ai_list.push_back(AICube);
}



void ACubeLevelScript::spawnAI()
{

    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Level-script: spawn"));

    for(int i=0; i<game_current_level * difficulty; ++i)
    {

        const FVector &random_location = FVector(FMath::RandRange(-arena_size.X, arena_size.X),
                                                 FMath::RandRange(-arena_size.Y, arena_size.Y), arena_size.Z);

        GetWorld()->SpawnActor<AAICube>(ai_cube_container, random_location, FRotator(0,0,0));
    }

    waitUntilTick(50);
    toBeExecuted = std::bind(&ACubeLevelScript::assignIdtoAI, this);

}



void ACubeLevelScript::waitUntilTick(const int &TickToWait)
{
    time_to_count = TickToWait;
}



void ACubeLevelScript::assignIdtoAI()
{

    static int id = 0;
    for(std::list<AAICube*>::iterator it=ai_list.begin(); it!=ai_list.end(); it++)
    {
        if((*it) != NULL)
            (*it)->assignAiId(id++);
    }

    id = 0;

    toBeExecuted = NULL;

}


