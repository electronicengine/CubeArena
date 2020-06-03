// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <thread>
#include <functional>
#include "Unix/UnixCriticalSection.h"
#include "Runtime/Core/Public/HAL/ThreadingBase.h"
#include "AICube.generated.h"


UCLASS()
class BOXARENA_API AAICube : public ACharacter
{
	GENERATED_BODY()

    class ACubeLevelScript *level_script;

    UPROPERTY(EditAnywhere)
    class UDestructibleComponent *destructable_component;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category=Projectile)
    TSubclassOf<class ABullet> bullet_container;

    FVector solid_color;

    void fireBullet();

    int dying_counter;
    int time_to_count;
    volatile float target_distance;

    std::function<void()> toBeExecuted;

public:
	// Sets default values for this character's properties
	AAICube();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;





public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void killTheCube(const FVector &ImpactPoint);
    void assignAiId(int Id);
    int getAiId();

    FVector &getSolidColor();

    void moveForward(float Value);
    void moveRight(float Value);
    void turn(FRotator &Rotation);
    void fire(float TimeToFire);
    void setTargetDistance(float Distance);

    UPROPERTY(EditAnywhere)
    int cube_id;
    volatile bool died;

};
