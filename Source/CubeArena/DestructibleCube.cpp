// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleCube.h"
#include "Components/StaticMeshComponent.h"



ADestructibleCube::ADestructibleCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

    GetWorld()->GetTimerManager().SetTimer(timer_handle_destroy, this, &ADestructibleCube::eraseAI, 2.0f, false);

}



// Sets default values
void ADestructibleCube::eraseAI()
{
    Destroy();
}
