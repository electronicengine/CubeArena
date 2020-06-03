// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include <functional>

#include "AICubeController.generated.h"

/**
 * 
 */
UCLASS()
class BOXARENA_API AAICubeController : public AAIController
{
	GENERATED_BODY()
	
	
private:

    UPROPERTY(EditAnywhere, Category = AI)
    float AISightRadius = 90.0f;

    UPROPERTY(EditAnywhere, Category = AI)
    float AISightAge = 5.0f;

    UPROPERTY(EditAnywhere, Category = AI)
    float AILoseSightRadius = AISightRadius + 50.0f;

    UPROPERTY(EditAnywhere, Category = AI)
    float AIFieldOfView = 90.0f;

    UPROPERTY(VisibleAnywhere, Category = AI)
    class UAISenseConfig_Sight* SightConfig;

    std::function<void(class AUserCube *)> toBeExecuted;
    int time_to_count;

    class AUserCube *user_cube;


    void getCloserToUser(class AUserCube *User);
    void fireToUser(class AUserCube *User);
    void turnAroundRight(class AUserCube *User);
    void turnAroundLeft(class AUserCube *User);
    int direction;
    int side;


    bool enemy_detected = false;

protected:
    virtual void OnPossess(APawn* Pawn) override;

public:

      AAICubeController();

      virtual void BeginPlay() override;


      virtual void Tick(float DeltaSeconds) override;


      UFUNCTION()
      void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);


};
