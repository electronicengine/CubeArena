// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UserCube.generated.h"

UCLASS()
class BOXARENA_API AUserCube : public ACharacter
{
	GENERATED_BODY()


    class ACubeLevelScript *level_script;

    UPROPERTY(EditAnywhere, Category = "Camera")
    class UCameraComponent *camera;

    UPROPERTY(EditAnywhere, Category = "Camera")
    class USpringArmComponent *spring_arm;

    UPROPERTY(EditAnywhere)
    class UDestructibleComponent *destructable_component;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float base_turn_rate;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float look_up_rate;

    UPROPERTY(EditAnywhere, Category = "Interaction")
    float trace_distance;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category=Projectile)
    TSubclassOf<class ABullet> bullet_container;

    void moveForward(float Value);
    void moveRight(float Value);
    void lookUpRate(float Value);
    void baseTurnRate(float Value);
    void fireBullet();


public:
	// Sets default values for this character's properties
	AUserCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
