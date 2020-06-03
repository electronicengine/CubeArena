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
#include "CubeLevelScript.generated.h"

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

    std::list<AAICube *> ai_list;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category=UserCube)
    class AUserCube *user_cube;

    int difficulty;
    int game_current_level;
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


    void attachUserToLevelScript(AUserCube *UserCube);

    void aiBulletHitCallBack();

    void userBulletHitCallBack(AActor *HitActor, const FVector ImpactPoint);

    void addList(AAICube *AICube);





	
};
