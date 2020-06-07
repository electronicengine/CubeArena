// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <thread>
#include <functional>
#include "AICube.generated.h"


#define FIRE_OFSETT_PITCH   2.0f

UCLASS()
class BOXARENA_API AAICube : public ACharacter
{
	GENERATED_BODY()

    class ACubeLevelScript *level_script;
    FTimerHandle timer_handle_destroy;
    FTimerHandle timer_handle_fire;


    UPROPERTY(EditAnywhere)
    class UDestructibleComponent *destructable_component;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category=Projectile)
    TSubclassOf<class ABullet> bullet_container;

    FVector solid_color;

    void eraseAI();
    bool died;


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

    void jump();
    void moveForward(float Value);
    void moveRight(float Value);
    void turnThatRotation(const FRotator &Rotation);
    void fireBullet();
    void setFireRate(const float &FireRate);
    float calculateDistanceTo(AActor *Actor);


    volatile float fire_rate;
    volatile bool fire_free;
    volatile int cube_id;

};
