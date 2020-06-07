// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "AICube.h"
#include "UserCube.h"
#include "Bullet.h"
#include <functional>
#include <list>
#include <iostream>
#include <vector>
#include "CubeLevelScript.generated.h"


#define LONGEST_FIRE_RATE   4
#define MIN_SPAWN_COUNT     1

/**
 * 
 */
UCLASS()
class BOXARENA_API ACubeLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()


    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category=AICube)
    TSubclassOf<class AAICube> ai_cube_container;

    UPROPERTY(EditDefaultsOnly, Category=AICube)
    TSubclassOf<class ADestructableCubes> destructible_cube_container;

    std::list<int> id_list;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category=UserCube)
    class AUserCube *user_cube;

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



protected:


    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    // Called every frame
    virtual void Tick(float DeltaTime) override;


public:

    ACubeLevelScript();

    void spawnDestrucableCubes();

    void attachUserToLevelScript(AUserCube *UserCube);

    void aiBulletHitCallBack();
    void userBulletHitCallBack(AAICube *AiCube, const FVector ImpactPoint);
    void destructibleHitCallBack();

    int takeId(AAICube *AICube);





	
};
