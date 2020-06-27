// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeLevelActor.h"
#include "Engine/Engine.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DestructibleObjects.h"
#include "UserPanel.h"
#include "CubeGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UserCube.h"
#include "TimerManager.h"
#include "BossCube.h"
#include "Engine/PostProcessVolume.h"
#include "Components/SkyLightComponent.h"


ACubeLevelActor::ACubeLevelActor() : arena_size{7000.0f, 7000.0f, 55.0f}
{

    PrimaryActorTick.bCanEverTick = true;

    difficulty = 2;
    game_current_level = 1;
    time_to_count = 0;
    user_point = 0;
    toBeExecuted = NULL;
    spawn_count = MIN_SPAWN_COUNT;
    fire_rate = LONGEST_FIRE_RATE;


    destructible_list.clear();


}



void ACubeLevelActor::BeginPlay()
{

    Super::BeginPlay();

    UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(), 0));

    user_cube = Cast<AUserCube>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    game_instance = Cast<UCubeGameInstance>(GetGameInstance());

    if(game_instance != NULL)
        user_panel = game_instance->loadUserPanel();

    if(user_cube != NULL)
    {
        user_cube->setUserPanel(user_panel);
        GetWorld()->GetTimerManager().SetTimer(user_cube->timer_handle, user_cube, &AUserCube::setGravityTreshold, 1.0f, false);
    }

    if(sky_light != NULL)
        sky_light->SetIntensity(Cast<UCubeGameInstance>(GetGameInstance())->getLightIntensity());

    toBeExecuted = std::bind(&ACubeLevelActor::spawnAI, this);

}



void ACubeLevelActor::spawnDestrucableCubes()
{


    ADestructibleObjects *destructible_object;
    const FVector &random_location = FVector(FMath::RandRange(-arena_size.X, arena_size.X),
                                             FMath::RandRange(-arena_size.Y, arena_size.Y), arena_size.Z);

    const FRotator &random_rotation = FRotator(0,
                                               FMath::RandRange(0.0, 360.0f),
                                               0);

    destructible_object = GetWorld()->SpawnActor<ADestructibleObjects>(destructible_container, random_location, random_rotation);

    destructible_list.push_back(destructible_object);


}



void ACubeLevelActor::spawnSuicideCube()
{
    const FVector &random_location = FVector(FMath::RandRange(-arena_size.X, arena_size.X),
                                             FMath::RandRange(-arena_size.Y, arena_size.Y), arena_size.Z);


    GetWorld()->SpawnActor<ASuicideCube>(suicide_cube_container, random_location, FRotator(0,0,0));
}



void ACubeLevelActor::spawnBossCube()
{
    const FVector &random_location = FVector(FMath::RandRange(-arena_size.X, arena_size.X),
                                             FMath::RandRange(-arena_size.Y, arena_size.Y), arena_size.Z);



    ABossCube * boss_cube = GetWorld()->SpawnActor<ABossCube>(boss_cube_container, random_location, FRotator(0,0,0));

    if(boss_cube != NULL)
        boss_cube->setFireRate(fire_rate);

}



void ACubeLevelActor::Tick(float DeltaTime)
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




void ACubeLevelActor::aiBulletHitCallBack()
{
    if(user_cube != NULL)
        user_cube->applyDamage();

}



void ACubeLevelActor::userBulletHitCallBack(AAICube *AiCube, const FVector ImpactPoint)
{



    if(AiCube != NULL)
    {
        int cube_id = AiCube->getAiId();

        std::list<int>::iterator it;


        for(it = id_list.begin(); it != id_list.end(); it++)
        {
            if(cube_id == (*it))
            {

                if(user_cube != NULL)
                {
                    user_cube->setSolidColor(AiCube->getSolidColor());
                    user_cube->setScoreAndBulletCap(1, 5);
                }



                if(AiCube != NULL)
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

            time_to_count = 400;    //wait some time
            toBeExecuted = std::bind(&ACubeLevelActor::spawnAI, this);
        }
    }

}



void ACubeLevelActor::suicideCubeHitCallBack()
{
    if(user_cube != NULL)
        user_cube->setScoreAndBulletCap(1, 3);

}



void ACubeLevelActor::destructibleHitCallBack()
{
    if(user_cube != NULL)
        user_cube->setScoreAndBulletCap(1,1);

}



void ACubeLevelActor::suicideCubeExplodeCallback()
{
    if(user_cube != NULL)
        user_cube->applyDamage();


}



void ACubeLevelActor::bossCubeHitCallBack()
{
    if(user_cube != NULL)
        user_cube->setScoreAndBulletCap(1, 3);

}



int ACubeLevelActor::takeId(AAICube *Cube)
{
    static int id = 0;

    id_list.push_back(++id);

    if(game_current_level % 3 != 0)
    {
        if(fire_rate <= 1.0f)
            fire_rate = 0.5f;
    }
    else
        fire_rate = LONGEST_FIRE_RATE;

    Cube->setFireRate(fire_rate);

    return id;
}



void ACubeLevelActor::spawnAI()
{

    AAICube *ai_cube;
    static int id = 0;

    if(game_current_level % 3 == 0)
    {
        spawn_count = difficulty * spawn_count;
        spawnBossCube();
    }

    for(int i = 0; i < destructible_list.size(); i++)
    {
        if(destructible_list[i] != NULL)
            destructible_list[i]->Destroy();
    }

    destructible_list.clear();


    for(int i=0; i<spawn_count; i++)
    {


        const FVector &random_location = FVector(FMath::RandRange(-arena_size.X, arena_size.X),
                                                 FMath::RandRange(-arena_size.Y, arena_size.Y), arena_size.Z);

        ai_cube = GetWorld()->SpawnActor<AAICube>(ai_cube_container, random_location, FRotator(0,0,0));

        spawnDestrucableCubes();
        spawnSuicideCube();

    }

    if(user_cube != NULL)
        user_cube->giveRank(game_current_level);

    waitUntilTick(50);

//    toBeExecuted = std::bind(&ACubeLevelScript::assignIdtoAI, this);
    toBeExecuted = NULL;

}



void ACubeLevelActor::waitUntilTick(const int &TickToWait)
{
    time_to_count = TickToWait;
}



void ACubeLevelActor::assignIdtoAI()
{
//    static int id = 0;

//    for(int i=0; i<ai_list.size(); i++)
//    {
//        id_list.push_back(id);
//        ai_list[i]->assignAiId(id++);
//    }



}

