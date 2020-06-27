// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SuicideCube.generated.h"

UCLASS()
class CUBEARENA_API ASuicideCube : public ACharacter
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category="Solid")
    class UStaticMeshComponent *cube_solid;

    UPROPERTY(EditAnywhere, Category="Solid")
    class UStaticMeshComponent *cube_face;

    FVector solid_color;

    class ACubeLevelActor *level_script;

    UPROPERTY(EditDefaultsOnly, Category=Destructible)
    TSubclassOf<class ADestructibleCube> destructible_container;

    UPROPERTY(EditAnywhere, Category=SpawnSound)
    class USoundCue *spawn_sound;

    UPROPERTY(EditAnywhere, Category=LoughSound)
    class USoundCue *jump_sound;

    UPROPERTY(EditDefaultsOnly, Category=Audio)
    class UAudioComponent *audio_component;

    bool died;

    void selectAFace();

    std::function<void()> toBeExecuted;

public:
	// Sets default values for this character's properties
	ASuicideCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void killTheCube(const FVector &ImpactPoint);
    void killTheCubeWithDamage(const FVector &ImpactPoint);

    void jump();
    void moveForward(float Value);
    void moveRight(float Value);
    void turnThatRotation(const FRotator &Rotation);
    float calculateDistanceTo(AActor *Actor);

	
};
