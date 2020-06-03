// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "CubeLevelScript.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DestructibleComponent.h"
#include "Engine/Engine.h"



// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    destructable_component = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructableComponent"));
    destructable_component->SetSimulatePhysics(true);
    destructable_component->SetNotifyRigidBodyCollision(true);
    destructable_component->BodyInstance.SetCollisionProfileName(TEXT("Bullet Colliion"));
    destructable_component->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
    destructable_component->SetupAttachment(RootComponent);


    // Use a ProjectileMovementComponent to govern this projectile's movement
    projectile_movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    projectile_movement->UpdatedComponent = RootComponent;
    projectile_movement->InitialSpeed = 6000.f;
    projectile_movement->MaxSpeed = 6000.f;
    projectile_movement->bRotationFollowsVelocity = true;
    projectile_movement->bShouldBounce = true;


    // Die after 3 seconds by default
    InitialLifeSpan = 300000.0f;
    time_to_destroy = 300.0f;
    time_counter = 0;

    destroyed = false;
    hit = false;


}



// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

    level_script = Cast<ACubeLevelScript>(GetWorld()->GetLevelScriptActor());

    destructable_component->CreateDynamicMaterialInstance(0);

    destructable_component->SetVectorParameterValueOnMaterials(FName("Color"), FVector(0.2f, 0.1f, 0.3f));

}



// Called every frame
void ABullet::Tick(float DeltaTime)
{

    Super::Tick(DeltaTime);

    if(destroyed == true)
    {
        ++time_counter;

        if(time_counter >= time_to_destroy)
        {
            bool ret = Destroy();

            if(ret == false)
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Bullet can not Destroyed"));

        }
    }



}



void ABullet::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{


    if((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
    {

        if(hit == false)
        {
            destroyed = true;
            hit = true;

            projectile_movement->StopMovementImmediately();


            AAICube *ai_cube = Cast<AAICube>(OtherActor);

            if(ai_cube != NULL)
            {

                level_script->userBulletHitCallBack(OtherActor, Hit.ImpactPoint);

            }

        }
    }
    else
    {

    }
}


