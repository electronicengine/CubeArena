// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleCube.generated.h"

UCLASS()
class CUBEARENA_API ADestructibleCube : public AActor
{
	GENERATED_BODY()

   TArray<class UStaticMeshComponent*> cube_comps;
   FVector solid_color;

   FTimerHandle timer_handle_destroy;
   FTimerHandle timer_handle_fire;

   UPROPERTY(EditAnywhere, Category=CrackSound)
   class USoundCue *crack_sound;

   UPROPERTY(EditDefaultsOnly, Category=Audio)
   class UAudioComponent *audio_component;

   void eraseAI();

public:	
	// Sets default values for this actor's properties
	ADestructibleCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    void setSolidColor(const FVector &Color);
    void expolode(const FVector &ImpactPoint, float Violence);
	
	
};
