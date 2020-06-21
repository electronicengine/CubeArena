// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include <functional>

#include "BossCubeAIController.generated.h"

#define FORWARD                     1
#define BACKWARD                    -1
#define GO_RIGHT                    1
#define FO_LEFT                     -1


/**
 * 
 */
UCLASS()
class CUBEARENA_API ABossCubeAIController : public AAIController
{
	GENERATED_BODY()
	
private:


   UPROPERTY(EditAnywhere, Category = AI)
   float AISightRadius = 16000.0f;

   UPROPERTY(EditAnywhere, Category = AI)
   float AISightAge = 5.0f;

   UPROPERTY(EditAnywhere, Category = AI)
   float AILoseSightRadius = 140.0f;

   UPROPERTY(EditAnywhere, Category = AI)
   float AIFieldOfView = 180.0f;

  UPROPERTY(VisibleAnywhere, Category = AI)
  class UAISenseConfig_Sight* SightConfig;

  std::function<void()> toBeExecuted;
  int time_togo_side;
  int time_togo_forward;

  class AUserCube *user_cube;
  float distance_to_close;

  float min_dist_to_close;
  float min_dist_to_fire;


  void keepDistanceFromUser();
  void getCloserToUser();
  void turnAround();

  int direction;
  int side;

  bool enemy_detected = false;

protected:
  virtual void OnPossess(APawn* Pawn) override;

public:

    ABossCubeAIController();

    virtual void BeginPlay() override;


    virtual void Tick(float DeltaSeconds) override;


    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	
	
};
