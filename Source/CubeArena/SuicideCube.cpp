// Fill out your copyright notice in the Description page of Project Settings.


#include "SuicideCube.h"
#include "Components/StaticMeshComponent.h"
#include "CubeLevelActor.h"
#include "Bullet.h"
#include "TimerManager.h"
#include "DestructibleCube.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "CubeGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"


// Sets default values
ASuicideCube::ASuicideCube() : solid_color(FMath::RandRange(0.0f, 0.3f),FMath::RandRange(0.0f, 0.2f), FMath::RandRange(0.0f, 0.2))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    cube_solid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeSolid"));
    cube_solid->SetupAttachment(RootComponent);

    cube_face = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AIFace"));
    cube_face->SetupAttachment(RootComponent);


    ConstructorHelpers::FObjectFinder<USoundCue> spawn_sound_cue(TEXT("SoundCue'/Game/sounds/SpawnSoundCue.SpawnSoundCue'"));
    if(spawn_sound_cue.Succeeded())
        spawn_sound = spawn_sound_cue.Object;

    ConstructorHelpers::FObjectFinder<USoundCue> jump_sound_cue(TEXT("SoundCue'/Game/sounds/JumpSoundCue.JumpSoundCue'"));
    if(jump_sound_cue.Succeeded())
        jump_sound = jump_sound_cue.Object;

    audio_component = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
    audio_component->SetupAttachment(RootComponent);

    died = false;

}


// Called when the game starts or when spawned
void ASuicideCube::BeginPlay()
{
	Super::BeginPlay();

    level_script = Cast<ACubeLevelActor>(GetWorld()->GetLevelScriptActor());

    cube_solid->CreateDynamicMaterialInstance(0);
    cube_solid->SetVectorParameterValueOnMaterials(FName("Color"), solid_color);

    cube_face->CreateDynamicMaterialInstance(0);
    cube_face->SetVectorParameterValueOnMaterials(FName("Color"), FVector(FMath::RandRange(0.0f, 0.2f),
                                                                        FMath::RandRange(0.0f, 0.2f),
                                                                        FMath::RandRange(0.0f, 0.2f)));

    audio_component->SetVolumeMultiplier(Cast<UCubeGameInstance>(GetGameInstance())->getSoundVolume());
    audio_component->SetSound(spawn_sound);
    audio_component->Play();

    selectAFace();

}


// Called every frame
void ASuicideCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// Called to bind functionality to input
void ASuicideCube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



// Sets default values
void ASuicideCube::selectAFace()
{
    UCubeGameInstance * game_instance = Cast<UCubeGameInstance>(GetGameInstance());

    cube_face->SetVectorParameterValueOnMaterials(FName("FaceParam"), game_instance->getface(FMath::RandRange(0, 24)));
}



void ASuicideCube::killTheCubeWithDamage(const FVector &ImpactPoint)
{

    FVector player_loc ;
    FRotator rotation;
    ADestructibleCube *destructible;

    level_script->suicideCubeExplodeCallback();

    if(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) != NULL)
    {
        player_loc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
        rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player_loc);
    }
    else
        rotation = FRotator(0,0,0);

    destructible = GetWorld()->SpawnActor<ADestructibleCube>(destructible_container, GetActorLocation(), rotation);
    destructible->setSolidColor(solid_color);
    Destroy();

    destructible->expolode(ImpactPoint, 1000.0f);

    died = true;


}

void ASuicideCube::killTheCube(const FVector &ImpactPoint)
{
    FVector player_loc ;
    FRotator rotation;
    ADestructibleCube *destructible;

    if(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) != NULL)
    {
        player_loc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
        rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player_loc);
    }
    else
        rotation = FRotator(0,0,0);

    destructible = GetWorld()->SpawnActor<ADestructibleCube>(destructible_container, GetActorLocation(), rotation);
    destructible->setSolidColor(solid_color);

    Destroy();

    destructible->expolode(ImpactPoint, 1000.0f);

    died = true;

}



void ASuicideCube::jump()
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



void ASuicideCube::moveForward(float Value)
{
    if((Controller) && Value != 0.0f && died != true)
    {
        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X);

        AddMovementInput(direction, Value);
    }
}



void ASuicideCube::moveRight(float Value)
{
    if((Controller) && Value != 0.0f && died != true)
    {
        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y);

        AddMovementInput(direction, Value);
    }
}



void ASuicideCube::turnThatRotation(const FRotator &Rotation)
{
    SetActorRotation(Rotation);
}



float ASuicideCube::calculateDistanceTo(AActor *Actor)
{
    return GetDistanceTo(Actor);
}

