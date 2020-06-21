// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleObjects.generated.h"

UCLASS()
class CUBEARENA_API ADestructibleObjects : public AActor
{
	GENERATED_BODY()

    class UStaticMeshComponent *cube_;
    class UStaticMeshComponent *cube_1;
    class UStaticMeshComponent *cube_2;
    class UStaticMeshComponent *coni_;
    class UStaticMeshComponent *sphere_;
    class UStaticMeshComponent *cylinder_;
    class UStaticMeshComponent *cylinder_1;

    FVector cube_color;
    FVector cube_1_color;
    FVector cube_2_color;
    FVector coni_color;
    FVector sphere_color;
    FVector cylinder_color;
    FVector cylinder_1_color;

    UPROPERTY(EditDefaultsOnly, Category=Destructible)
    TSubclassOf<class ADestructibleCube> destructible_container;

    UPROPERTY(EditDefaultsOnly, Category=Heal)
    TSubclassOf<class AHealActor> heal_container;

    TArray<class UStaticMeshComponent*> mesh_components;
    TArray<FVector> colors;
    int destructible_object_number;


public:	
	// Sets default values for this actor's properties
	ADestructibleObjects();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


    void destroyComp(UPrimitiveComponent *OtherComp, const FVector &ImpactPoint);

};
