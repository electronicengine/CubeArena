// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructableCubes.h"
#include "DestructibleComponent.h"
#include "Engine/Engine.h"
#include "Components/SceneComponent.h"
#include <string>
#include "TimerManager.h"

// Sets default values
ADestructableCubes::ADestructableCubes()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    died_count = 0;
//        scene_component = CreateDefaultSubobject<USceneComponent>(TEXT("ScheneComponet"));
//        RootComponent = scene_component;

//        destructible_component_0 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("destructible_component_0"));
//        destructible_component_0->SetSimulatePhysics(true);
//        destructible_component_0->SetNotifyRigidBodyCollision(true);
//        destructible_component_0->BodyInstance.SetCollisionProfileName(TEXT("Destructible Collision_0"));
//        destructible_component_0->SetupAttachment(RootComponent);

//        destructible_component_1 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("destructible_component_1"));
//        destructible_component_1->SetSimulatePhysics(true);
//        destructible_component_1->SetNotifyRigidBodyCollision(true);
//        destructible_component_1->BodyInstance.SetCollisionProfileName(TEXT("Destructible Collision_1"));
//        destructible_component_1->SetupAttachment(RootComponent);

//        destructible_component_2 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("destructible_component_2"));
//        destructible_component_2->SetSimulatePhysics(true);
//        destructible_component_2->SetNotifyRigidBodyCollision(true);
//        destructible_component_2->BodyInstance.SetCollisionProfileName(TEXT("Destructible Collision_2"));
//        destructible_component_2->SetupAttachment(RootComponent);

//        destructible_component_3 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("destructible_component_3"));
//        destructible_component_3->SetSimulatePhysics(true);
//        destructible_component_3->SetNotifyRigidBodyCollision(true);
//        destructible_component_3->BodyInstance.SetCollisionProfileName(TEXT("Destructible Collision_3"));
//        destructible_component_3->SetupAttachment(RootComponent);

    //    destructible_component_4 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("destructible_component_4"));
    //    destructible_component_4->SetSimulatePhysics(true);
    //    destructible_component_4->SetNotifyRigidBodyCollision(true);
    //    destructible_component_4->BodyInstance.SetCollisionProfileName(TEXT("Destructible Collision_4"));
    //    destructible_component_4->SetupAttachment(RootComponent);

    //    destructible_component_5 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("destructible_component_5"));
    //    destructible_component_5->SetSimulatePhysics(true);
    //    destructible_component_5->SetNotifyRigidBodyCollision(true);
    //    destructible_component_5->BodyInstance.SetCollisionProfileName(TEXT("Destructible Collision_5"));
    //    destructible_component_5->SetupAttachment(RootComponent);



}



// Called when the game starts or when spawned
void ADestructableCubes::BeginPlay()
{
	Super::BeginPlay();

    TArray<UDestructibleComponent*> comps;

    this->GetComponents(comps);
    for(auto destructibles : comps)
    {
        destructibles->CreateDynamicMaterialInstance(0);
        destructibles->SetVectorParameterValueOnMaterials(FName("Color"), FVector(FMath::RandRange(0.0f, 0.3f),
                                                                                        FMath::RandRange(0.0f, 0.3f),
                                                                                        FMath::RandRange(0.0f, 0.3f)));
    }


//    destructible_component_0->CreateDynamicMaterialInstance(0);
//    destructible_component_0->SetVectorParameterValueOnMaterials(FName("Color"), FVector(FMath::RandRange(0.0f, 0.3f),
//                                                                                  FMath::RandRange(0.0f, 0.3f),
//                                                                                  FMath::RandRange(0.0f, 0.3f)));
//    destructible_component_1->CreateDynamicMaterialInstance(0);
//    destructible_component_1->SetVectorParameterValueOnMaterials(FName("Color"), FVector(FMath::RandRange(0.0f, 0.3f),
//                                                                                  FMath::RandRange(0.0f, 0.3f),
//                                                                                  FMath::RandRange(0.0f, 0.3f)));
//    destructible_component_2->CreateDynamicMaterialInstance(0);
//    destructible_component_2->SetVectorParameterValueOnMaterials(FName("Color"), FVector(FMath::RandRange(0.0f, 0.3f),
//                                                                                  FMath::RandRange(0.0f, 0.3f),
//                                                                                  FMath::RandRange(0.0f, 0.3f)));
//    destructible_component_3->CreateDynamicMaterialInstance(0);
//    destructible_component_3->SetVectorParameterValueOnMaterials(FName("Color"), FVector(FMath::RandRange(0.0f, 0.3f),
//                                                                                  FMath::RandRange(0.0f, 0.3f),
//                                                                                  FMath::RandRange(0.0f, 0.3f)));
//    destructible_component_4->CreateDynamicMaterialInstance(0);
//    destructible_component_4->SetVectorParameterValueOnMaterials(FName("Color"), FVector(FMath::RandRange(0.0f, 0.3f),
//                                                                                  FMath::RandRange(0.0f, 0.3f),
//                                                                                  FMath::RandRange(0.0f, 0.3f)));
//    destructible_component_5->CreateDynamicMaterialInstance(0);
//    destructible_component_5->SetVectorParameterValueOnMaterials(FName("Color"), FVector(FMath::RandRange(0.0f, 0.3f),
//                                                                                  FMath::RandRange(0.0f, 0.3f),
//                                                                                   FMath::RandRange(0.0f, 0.3f)));

}



// Called every frame
void ADestructableCubes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ADestructableCubes::killDestructibles()
{
    GetWorld()->GetTimerManager().SetTimer(timer_handle, this, &ADestructableCubes::erase, 10.0f, false);

}



void ADestructableCubes::erase()
{
    Destroy();
}


