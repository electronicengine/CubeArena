// Fill out your copyright notice in the Description page of Project Settings.


#include "HealActor.h"
#include "Components/StaticMeshComponent.h"
#include "CubeLevelActor.h"
#include "Bullet.h"
#include "TimerManager.h"
#include "DestructibleCube.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "CubeGameInstance.h"
#include "Engine/Engine.h"

// Sets default values
AHealActor::AHealActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

//    heal_solid = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(FName("HealSolid")));
    heal_solid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealSolid"));
    heal_solid->SetupAttachment(RootComponent);

    heal_solid->OnComponentBeginOverlap.AddDynamic(this, &AHealActor::OnOverlapBegin);

    InitialLifeSpan = 10.0f;

}



// Called when the game starts or when spawned
void AHealActor::BeginPlay()
{
	Super::BeginPlay();



	
}



// Called every frame
void AHealActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    AddActorLocalRotation(FRotator(0.0f, DeltaTime + 1.0f, 0.0f));


}



void AHealActor::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    AUserCube *user_cube = Cast<AUserCube>(OtherActor);

    if(user_cube != NULL)
        user_cube->setHeal();

    Destroy();
}

