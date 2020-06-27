// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "AICube.h"
#include "UserCube.h"
#include "SuicideCube.h"
#include "Bullet.h"
#include <functional>
#include <list>
#include <iostream>
#include <vector>

#include "CubeLevelActor.generated.h"

#define LONGEST_FIRE_RATE   4
#define MIN_SPAWN_COUNT     1

/**
 * 
 */
UCLASS()
class CUBEARENA_API ACubeLevelActor : public ALevelScriptActor
{
	GENERATED_BODY()

     /** Projectile class to spawn */
     UPROPERTY(EditDefaultsOnly, Category=AICube)
     TSubclassOf<class AAICube> ai_cube_container;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category=BossCube)
    TSubclassOf<class ABossCube> boss_cube_container;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category=SuicideCube)
    TSubclassOf<class ASuicideCube> suicide_cube_container;

    /** Destrcutible class to spawn */
    UPROPERTY(EditDefaultsOnly, Category=AICube)
    TSubclassOf<class ADestructibleObjects> destructible_container;

     std::list<int> id_list;
     std::vector<class ADestructibleObjects *> destructible_list;

     /** Projectile class to spawn */
     UPROPERTY(EditDefaultsOnly, Category=UserCube)
     class AUserCube *user_cube;

     UPROPERTY(EditAnywhere, Category=SkyLight)
     class USkyLightComponent *sky_light;


     int difficulty;
     int game_current_level;
     float fire_rate;
     int spawn_count;
     int time_to_count;
     int user_point;



     void waitUntilTick(const int &TickToWait);
     void spawnAI();
     void assignIdtoAI();

     std::function<void()> toBeExecuted;

     const FVector arena_size;
    class UCubeGameInstance *game_instance;
    class UUserWidget *user_panel;

 protected:


     // Called when the game starts or when spawned
     virtual void BeginPlay() override;
     // Called every frame
     virtual void Tick(float DeltaTime) override;


 public:

     ACubeLevelActor();

     void spawnDestrucableCubes();
     void spawnSuicideCube();
     void spawnBossCube();

     void aiBulletHitCallBack();
     void userBulletHitCallBack(AAICube *AiCube, const FVector ImpactPoint);
     void destructibleHitCallBack();
     void suicideCubeExplodeCallback();
     void bossCubeHitCallBack();
     void suicideCubeHitCallBack();

     int takeId(AAICube *AICube);



	
	
};
