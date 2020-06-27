// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "CubeLevelActor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "DestructibleObjects.h"
#include "SuicideCube.h"
#include "BossCube.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "CubeGameInstance.h"


// Sets default values
ABullet::ABullet()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle
            (TEXT("/Game/particles/P_Explosion.P_Explosion"));
            ParticleFX1 = Particle.Object;

    bullet_solid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bullet_solid"));
    bullet_solid->SetSimulatePhysics(true);
    bullet_solid->SetNotifyRigidBodyCollision(true);
    bullet_solid->BodyInstance.SetCollisionProfileName(TEXT("Bullet Colliion"));
    bullet_solid->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
    bullet_solid->SetupAttachment(RootComponent);


    // Use a ProjectileMovementComponent to govern this projectile's movement
    projectile_movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    projectile_movement->UpdatedComponent = RootComponent;
    projectile_movement->InitialSpeed = 6000.f;
    projectile_movement->MaxSpeed = 6000.f;
    projectile_movement->bRotationFollowsVelocity = true;
    projectile_movement->bShouldBounce = true;


    ConstructorHelpers::FObjectFinder<USoundCue> popping_sound_cue(TEXT("SoundCue'/Game/sounds/PoppingSoundCue.PoppingSoundCue'"));
    if(popping_sound_cue.Succeeded())
        popping_sound = popping_sound_cue.Object;

    audio_component = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
    audio_component->SetupAttachment(RootComponent);


    // Die after 3 seconds by default
    InitialLifeSpan = 300000.0f;
    time_to_destroy = 300.0f;
    time_counter = 0;

    destroyed = false;
    hit = false;
    example = 0;


}


// Called when the game starts or when spawned
void ABullet::BeginPlay()
{

    Super::BeginPlay();

    level_script = Cast<ACubeLevelActor>(GetWorld()->GetLevelScriptActor());
    bullet_solid->CreateDynamicMaterialInstance(0);
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
            Destroy();

//            if(ret == false)
//                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Bullet can not Destroyed"));

        }
    }

}



void ABullet::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{

    if((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
    {


        if(hit == false && destroyed == false)
        {

            audio_component->SetSound(popping_sound);
            audio_component->SetVolumeMultiplier(Cast<UCubeGameInstance>(GetGameInstance())->getSoundVolume());
            audio_component->Play();

            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleFX1, GetActorLocation());

            destroyed = true;
            hit = true;

            projectile_movement->StopMovementImmediately();

            AAICube *ai_cube = Cast<AAICube>(OtherActor);

            if(ai_cube != NULL && Cast<AAICube>(bullet_owner) == NULL) // hitting ai_cube
            {

                level_script->userBulletHitCallBack(ai_cube, Hit.ImpactPoint);

            }
            else
            {
                AUserCube *user_cube = Cast<AUserCube>(OtherActor);

                if(user_cube != NULL && Cast<AUserCube>(bullet_owner) == NULL) // hitting user_cube
                {
                    level_script->aiBulletHitCallBack();

                    if(Cast<AAICube>(bullet_owner) != NULL)
                        Cast<AAICube>(bullet_owner)->selectLaughFace();

                }
                else
                {

                    ADestructibleObjects * destructable_objects = Cast<ADestructibleObjects>(OtherActor);
                    if(destructable_objects != NULL) // hitting destructibles
                    {
                        destructable_objects->destroyComp(OtherComp, Hit.ImpactPoint);
                        level_script->destructibleHitCallBack();

                    }
                    else
                    {
                        ASuicideCube *suicide_cube = Cast<ASuicideCube>(OtherActor);
                        if(suicide_cube != NULL) // hitting suicide cube
                        {
                            suicide_cube->killTheCube(Hit.ImpactPoint);
                            level_script->suicideCubeHitCallBack();

                        }
                        else
                        {
                            ABossCube *boss_cube = Cast<ABossCube>(OtherActor);
                            if(boss_cube != NULL) // hitting boss cube
                            {
                                boss_cube->killTheCube(Hit.ImpactPoint);
                                level_script->bossCubeHitCallBack();
                            }

                        }

                    }

                }
            }

        }
    }
    else
    {

    }
}



void ABullet::setBulletOwner(AActor *Actor)
{
    bullet_owner = Actor;
}



AActor *ABullet::getBulletOwner()
{
    return bullet_owner;
}



void ABullet::setSolidColor(const FVector &Color)
{
    solid_color = Color;
    bullet_solid->SetVectorParameterValueOnMaterials(FName("Color"), solid_color);
}



FVector *ABullet::getSolidColor()
{
    return &solid_color;
}
