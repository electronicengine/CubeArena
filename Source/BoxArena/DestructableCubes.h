// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructableCubes.generated.h"

UCLASS()
class BOXARENA_API ADestructableCubes : public AActor
{
	GENERATED_BODY()


    FTimerHandle timer_handle;

public:	
	// Sets default values for this actor's properties
	ADestructableCubes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    volatile int died_count;

    void killDestructibles();
    void erase();

};
