// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCube.h"
#include "Components/StaticMeshComponent.h"
#include "CubeLevelActor.h"
#include "Bullet.h"
#include "TimerManager.h"
#include "DestructibleCube.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "CubeGameInstance.h"
#include "HealActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
ABossCube::ABossCube() : solid_color(FMath::RandRange(0.0f, 0.2f), FMath::RandRange(0.0f, 0.2f), FMath::RandRange(0.0f, 0.2f))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;


    ConstructorHelpers::FObjectFinder<USoundCue> spawn_sound_cue(TEXT("SoundCue'/Game/sounds/SpawnSoundCue.SpawnSoundCue'"));
    if(spawn_sound_cue.Succeeded())
        spawn_sound = spawn_sound_cue.Object;


    ConstructorHelpers::FObjectFinder<USoundCue> jump_sound_cue(TEXT("SoundCue'/Game/sounds/JumpSoundCue.JumpSoundCue'"));
    if(jump_sound_cue.Succeeded())
        jump_sound = jump_sound_cue.Object;

    audio_component = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
    audio_component->SetupAttachment(RootComponent);

    died = false;
    fire_free = false;
    fire_accurancy = 20.0f;

    hit_count = 10;



}

// Called when the game starts or when spawned
void ABossCube::BeginPlay()
{
    Super::BeginPlay();

    boss_solid = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(FName("BossSolid")));
    boss_face = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(FName("BossFace")));


    level_script = Cast<ACubeLevelActor>(GetWorld()->GetLevelScriptActor());

//    cube_id = level_script->takeId(this);

    boss_solid->CreateDynamicMaterialInstance(0);
    boss_solid->SetVectorParameterValueOnMaterials(FName("Color"), solid_color);

    audio_component->SetVolumeMultiplier(Cast<UCubeGameInstance>(GetGameInstance())->getSoundVolume());
    audio_component->SetSound(spawn_sound);
    audio_component->Play();

    selectAFace();
	
}

// Called every frame
void ABossCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


// Sets default values
void ABossCube::selectAFace()
{
    UCubeGameInstance * game_instance = Cast<UCubeGameInstance>(GetGameInstance());

    boss_face->CreateDynamicMaterialInstance(0);
    boss_face->SetVectorParameterValueOnMaterials(FName("Color"), FVector(1.0f, 1.0f, 1.0f));

    boss_face->SetVectorParameterValueOnMaterials(FName("FaceParam"), game_instance->getface(FMath::RandRange(0, 24)));
}



void ABossCube::killTheCube(const FVector &ImpactPoint)
{

    hit_count--;

    if(hit_count <= 0)
    {
        FVector player_loc;
        const FVector &actor_loc = GetActorLocation();
        FRotator rotation;
        ADestructibleCube *destructible;

        if(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) != NULL)
        {
            player_loc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
            rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player_loc);
        }
        else
            rotation = FRotator(0,0,0);

        destructible = GetWorld()->SpawnActor<ADestructibleCube>(destructible_container, actor_loc, rotation);
        destructible->setSolidColor(solid_color);
        destructible->SetActorScale3D(FVector(2.4f, 2.4f, 2.4f));

        died = true;
        destructible->expolode(ImpactPoint, 1000.0f);

        if(FMath::RandRange(0, 10) % 10 == 0)
            GetWorld()->SpawnActor<AHealActor>(heal_container, FVector(actor_loc.X, actor_loc.Y, 90.0f), FRotator(0,0,0));

        Destroy();
    }


}



void ABossCube::assignAiId(int Id)
{

    cube_id = Id;

//    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Cube Assigned ID: ") + FString::FromInt(cube_id));

}



void ABossCube::moveForward(float Value)
{
    if((Controller) && Value != 0.0f && died != true)
    {
        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X);

        AddMovementInput(direction, Value);
    }
}



void ABossCube::moveRight(float Value)
{
    if((Controller) && Value != 0.0f && died != true)
    {
        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y);

        AddMovementInput(direction, Value);
    }
}



void ABossCube::fireBullet()
{

    ABullet *bullet;

    if (bullet_container != NULL && died != true && fire_free == true)
    {
        UWorld* const World = GetWorld();
        if (World != NULL)
        {

            FRotator spawn_rotation;
            FVector spawn_location;


            spawn_rotation= GetActorRotation();
            spawn_location = GetActorLocation() + (spawn_rotation.Vector() * 90) ;


            spawn_rotation.Pitch = spawn_rotation.Pitch + FIRE_OFSETT_PITCH;
            spawn_rotation.Yaw = spawn_rotation.Yaw + FMath::RandRange(-fire_accurancy, fire_accurancy);

            bullet = World->SpawnActor<ABullet>(bullet_container, spawn_location, spawn_rotation);
            bullet->setSolidColor(solid_color);
            bullet->setBulletOwner(this);

        }
    }
}


void ABossCube::setFireRate(const float &FireRate)
{


    GetWorld()->GetTimerManager().ClearTimer(timer_handle_fire);
    GetWorld()->GetTimerManager().SetTimer(timer_handle_fire, this, &ABossCube::fireBullet, FireRate + FMath::RandRange(0.0f, 0.5f), true);
}



float ABossCube::calculateDistanceTo(AActor *Actor)
{
    return GetDistanceTo(Actor);
}



int ABossCube::getAiId()
{
    int value = cube_id;

    return value;
}



FVector &ABossCube::getSolidColor()
{
    return solid_color;
}



void ABossCube::jump()
{
    if(died != true)
    {
        if(!audio_component->IsPlaying())
        {
            audio_component->SetSound(jump_sound);
            audio_component->Play();
        }
        Jump();
    }

}



void ABossCube::turnThatRotation(const FRotator &Rotation)
{
    SetActorRotation(Rotation);
}


