// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICube.generated.h"

#define FIRE_OFSETT_PITCH   2.0f


UCLASS()
class CUBEARENA_API AAICube : public ACharacter
{
	GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category=Solid)
    class UStaticMeshComponent *ai_solid;

    UPROPERTY(EditDefaultsOnly, Category=Face)
    class UStaticMeshComponent *ai_face;

    FVector solid_color;

    class ACubeLevelActor *level_script;
    FTimerHandle timer_handle_fire;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category=Projectile)
    TSubclassOf<class ABullet> bullet_container;

    UPROPERTY(EditDefaultsOnly, Category=Destructible)
    TSubclassOf<class ADestructibleCube> destructible_container;

    UPROPERTY(EditAnywhere, Category=SpawnSound)
    class USoundCue *spawn_sound;

    UPROPERTY(EditAnywhere, Category=LoughSound)
    class USoundCue *lough_sound;

    UPROPERTY(EditAnywhere, Category=LoughSound)
    class USoundCue *jump_sound;

    UPROPERTY(EditAnywhere, Category=FireSound)
    class USoundCue *fire_sound;

    UPROPERTY(EditDefaultsOnly, Category=Audio)
    class UAudioComponent *audio_component;

    bool died;


    std::function<void()> toBeExecuted;

    void selectAFace();

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
    void selectLaughFace();


    volatile float fire_rate;
    volatile bool fire_free;
    volatile int cube_id;
    volatile float fire_accurancy;

	
};
