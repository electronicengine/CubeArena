// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleCube.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "CubeGameInstance.h"

ADestructibleCube::ADestructibleCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    ConstructorHelpers::FObjectFinder<USoundCue> crack_sound_cue(TEXT("SoundCue'/Game/sounds/CrackSoundCue.CrackSoundCue'"));
    if(crack_sound_cue.Succeeded())
        crack_sound = crack_sound_cue.Object;

    audio_component = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    audio_component->SetupAttachment(RootComponent);
}



// Called when the game starts or when spawned
void ADestructibleCube::BeginPlay()
{
	Super::BeginPlay();

    this->GetComponents(cube_comps);
    for(auto meshes : cube_comps)
    {
        meshes->CreateDynamicMaterialInstance(0);
    }

	
}



// Called every frame
void ADestructibleCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ADestructibleCube::setSolidColor(const FVector &Color)
{
    solid_color = Color;
    for(auto meshes : cube_comps)
    {
        meshes->SetVectorParameterValueOnMaterials(FName("Color"), solid_color);

    }
}



void ADestructibleCube::expolode(const FVector &ImpactPoint, float Violence)
{
    for(UStaticMeshComponent *meshes: cube_comps)
    {
        meshes->SetSimulatePhysics(true);
        meshes->AddRadialImpulse(ImpactPoint, Violence, Violence, RIF_Constant, true);
    }

    GetWorld()->GetTimerManager().SetTimer(timer_handle_destroy, this, &ADestructibleCube::eraseAI, 5.0f, false);

    audio_component->SetVolumeMultiplier(Cast<UCubeGameInstance>(GetGameInstance())->getSoundVolume());
    audio_component->SetSound(crack_sound);
    audio_component->Play();
}



// Sets default values
void ADestructibleCube::eraseAI()
{
    Destroy();
}
