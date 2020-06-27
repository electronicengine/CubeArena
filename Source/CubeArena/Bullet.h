// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class CUBEARENA_API ABullet : public AActor
{
	GENERATED_BODY()


    class ACubeLevelActor *level_script;

    UPROPERTY(EditAnywhere, Category = "particle")
    class UParticleSystem *ParticleFX1;

    UPROPERTY(VisibleAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
    class UProjectileMovementComponent *projectile_movement;

    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent *bullet_solid;

    UPROPERTY(EditAnywhere, Category=PoppingSound)
    class USoundCue *popping_sound;

    UPROPERTY(EditDefaultsOnly, Category=Audio)
    class UAudioComponent *audio_component;

    FVector solid_color;
    AActor *bullet_owner;

    volatile bool destroyed;
    volatile bool hit;
    float time_to_destroy;
    int time_counter;

    volatile float example;
	

public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);
    /** called when something enters the sphere component */

    void setBulletOwner(AActor *Actor);
    AActor *getBulletOwner();

    void setSolidColor(const FVector &Color);
    FVector *getSolidColor();

	
};
