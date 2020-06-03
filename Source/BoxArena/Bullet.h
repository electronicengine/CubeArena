// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

enum class BulletOwner
{
    ai,
    user

};

UCLASS()
class BOXARENA_API ABullet : public AActor
{
	GENERATED_BODY()



    class ACubeLevelScript *level_script;
	
    UPROPERTY(VisibleAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
    class UProjectileMovementComponent *projectile_movement;

    UPROPERTY(EditAnywhere)
    class UDestructibleComponent *destructable_component;

    FVector solid_color;
    AActor *bullet_owner;

//    UPROPERTY(EditAnywhere)
//    class UStaticMeshComponent *static_mesh;

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
