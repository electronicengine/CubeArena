// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealActor.generated.h"

UCLASS()
class CUBEARENA_API AHealActor : public AActor
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category=Solid)
    class UStaticMeshComponent *heal_solid;

	
public:	
	// Sets default values for this actor's properties
	AHealActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
};
