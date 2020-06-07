// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeLevelScript.h"
#include "Engine/Engine.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DestructableCubes.h"


ACubeLevelScript::ACubeLevelScript() : arena_size{7000.0f, 7000.0f, 55.0f}
{

    PrimaryActorTick.bCanEverTick = true;

    difficulty = 2;
    game_current_level = 1;
    time_to_count = 0;
    user_point = 0;
    toBeExecuted = NULL;
    spawn_count = MIN_SPAWN_COUNT;
    fire_rate = LONGEST_FIRE_RATE +1;

}



void ACubeLevelScript::spawnDestrucableCubes()
{
    for(int i=0; i<4; i++)
    {


        const FVector &random_location = FVector(FMath::RandRange(-7000.0f, 7000.0f),
                                                 FMath::RandRange(-7000.0f, 7000.0f), arena_size.Z);

        const FRotator &random_rotation = FRotator(0,
                                                   FMath::RandRange(0.0, 360.0f),
                                                   0);

        GetWorld()->SpawnActor<ADestructableCubes>(destructible_cube_container, random_location, random_rotation);


    }
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
//    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Level-script: spawn"));

    toBeExecuted = std::bind(&ACubeLevelScript::spawnAI, this);

}



void ACubeLevelScript::attachUserToLevelScript(AUserCube *UserCube)
{

    user_cube = UserCube;
//    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("attachUserToLevelScript"));

}



void ACubeLevelScript::aiBulletHitCallBack()
{
    user_cube->applyDamage();
}



void ACubeLevelScript::userBulletHitCallBack(AAICube *AiCube, const FVector ImpactPoint)
{



    if(AiCube != NULL)
    {
        int cube_id = AiCube->getAiId();

        std::list<int>::iterator it;


        for(it = id_list.begin(); it != id_list.end(); it++)
        {
            if(cube_id == (*it))
            {

                user_cube->setSolidColor(AiCube->getSolidColor());
                user_cube->setScoreAndBulletCap(1, 5);

                AiCube->killTheCube(ImpactPoint);


                id_list.erase(it++);
//                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Left AICube Num: ") + FString::FromInt(id_list.size()));

            }
        }

        if(id_list.size() == 0)
        {
//            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Next Level "));
            game_current_level++;

            id_list.clear();

            toBeExecuted = std::bind(&ACubeLevelScript::spawnAI, this);
        }
    }

}



void ACubeLevelScript::destructibleHitCallBack()
{
//    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Destructible Hit"));
user_cube->setScoreAndBulletCap(1, 5);
}



int ACubeLevelScript::takeId(AAICube *Cube)
{
    static int id = 0;

    id_list.push_back(++id);

    if(game_current_level % 3 != 0)
    {
        fire_rate--;
        if(fire_rate <= 1.0f)
            fire_rate = 0.5f;
    }
    else
        fire_rate = LONGEST_FIRE_RATE;

    Cube->setFireRate(fire_rate);

    return id;
}



void ACubeLevelScript::spawnAI()
{

    AAICube *ai_cube;
    static int id = 0;

    if(game_current_level % 3 == 0)
        spawn_count = difficulty * spawn_count;

    for(int i=0; i<spawn_count; i++)
    {


        const FVector &random_location = FVector(FMath::RandRange(-arena_size.X, arena_size.X),
                                                 FMath::RandRange(-arena_size.Y, arena_size.Y), arena_size.Z);

        ai_cube = GetWorld()->SpawnActor<AAICube>(ai_cube_container, random_location, FRotator(0,0,0));


    }

    spawnDestrucableCubes();
    waitUntilTick(50);

//    toBeExecuted = std::bind(&ACubeLevelScript::assignIdtoAI, this);
    toBeExecuted = NULL;

}



void ACubeLevelScript::waitUntilTick(const int &TickToWait)
{
    time_to_count = TickToWait;
}



void ACubeLevelScript::assignIdtoAI()
{
//    static int id = 0;

//    for(int i=0; i<ai_list.size(); i++)
//    {
//        id_list.push_back(id);
//        ai_list[i]->assignAiId(id++);
//    }



}


