// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleObjects.h"
#include "Components/StaticMeshComponent.h"
#include "CubeLevelActor.h"
#include "Bullet.h"
#include "TimerManager.h"
#include "DestructibleCube.h"
#include "Containers/Array.h"
#include "Containers/TransArray.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "HealActor.h"


// Sets default values
ADestructibleObjects::ADestructibleObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;


    destructible_object_number = 12;



}



// Called when the game starts or when spawned
void ADestructibleObjects::BeginPlay()
{
	Super::BeginPlay();
    FVector random_color ;

    this->GetComponents(mesh_components);
    for(int i= 0; i<mesh_components.Num(); i++)
    {
        random_color = FVector(FMath::RandRange(0.0f, 0.3f),FMath::RandRange(0.0f, 0.2f), FMath::RandRange(0.0f, 0.2));
        mesh_components[i]->CreateDynamicMaterialInstance(0);
        mesh_components[i]->SetVectorParameterValueOnMaterials(FName("Color"), random_color);
        colors.Push(random_color);
    }


	
}



// Called every frame
void ADestructibleObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ADestructibleObjects::destroyComp(UPrimitiveComponent *OtherComp, const FVector &ImpactPoint)
{

    ADestructibleCube *destructible;
    FVector loc = OtherComp->GetComponentLocation();
    FRotator rot = OtherComp->GetComponentRotation();
    FVector color;
    FVector player_loc;
    FRotator rotation;

    destructible_object_number--;

    for(int i= 0; i<mesh_components.Num(); i++)
    {
        if(mesh_components[i] == OtherComp)
            color = colors[i];
    }



    if(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) != NULL)
    {
        player_loc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
        rotation = UKismetMathLibrary::FindLookAtRotation(loc, player_loc);

        destructible = GetWorld()->SpawnActor<ADestructibleCube>(destructible_container, loc, rotation);
        destructible->setSolidColor(color);
        destructible->expolode(ImpactPoint, 500.0f);

        OtherComp->DestroyComponent();

        if(FMath::RandRange(0, 120) % 120 == 0)
            GetWorld()->SpawnActor<AHealActor>(heal_container, FVector(loc.X, loc.Y, 100.0f), FRotator(0,0,0));

        if(destructible_object_number == 0)
            Destroy();
    }

}




