// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCube.generated.h"

UCLASS()
class CUBEARENA_API ABossCube : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCube();

    class UStaticMeshComponent *boss_solid;
    class UStaticMeshComponent *boss_face;

    FVector solid_color;

    class ACubeLevelActor *level_script;
    FTimerHandle timer_handle_fire;

    /** Projectile class to spawn */
    UPROPERTY(EditAnywhere, Category=Projectile)
    TSubclassOf<class ABullet> bullet_container;

    UPROPERTY(EditAnywhere, Category=Destructible)
    TSubclassOf<class ADestructibleCube> destructible_container;

    UPROPERTY(EditAnywhere, Category=Heal)
    TSubclassOf<class AHealActor> heal_container;


    bool died;

    int hit_count;

    std::function<void()> toBeExecuted;

    void selectAFace();

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
    volatile float fire_accurancy;
	
};
