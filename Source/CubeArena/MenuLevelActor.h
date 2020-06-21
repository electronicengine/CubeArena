// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MenuLevelActor.generated.h"

/**
 * 
 */
UCLASS()
class CUBEARENA_API AMenuLevelActor : public ALevelScriptActor
{
	GENERATED_BODY()
	

    class UCubeGameInstance *game_instance;
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintImplementableEvent, Category="Camera")
    void setCameraTarget();


};
