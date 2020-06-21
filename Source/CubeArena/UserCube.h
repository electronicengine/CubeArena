// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UserCube.generated.h"

UCLASS()
class CUBEARENA_API AUserCube : public ACharacter
{
	GENERATED_BODY()

    FVector2D port_size;
    float middle_of_screen_x;
    class ACubeLevelActor *level_script;

    UPROPERTY(EditAnywhere, Category = "Camera")
    class UCameraComponent *camera;

    UPROPERTY(EditAnywhere, Category = "Camera")
    class USpringArmComponent *spring_arm;

    UPROPERTY(EditAnywhere, Category = "UserSolid")
    class UStaticMeshComponent *user_solid;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float base_turn_rate;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float look_up_rate;

    UPROPERTY(EditAnywhere, Category = "Interaction")
    float trace_distance;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category=Projectile)
    TSubclassOf<class ABullet> bullet_container;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> user_panel_container;

    // Internal reference to the blueprint for gameplay logic
    class UUserWidget* user_panel;

    APlayerController *player_controller;
    FVector  solid_color;
    float gravity_forward_treshold;
    float gravity_backward_treshold;

    void moveForward(float Value);
    void moveRight(float Value);
    void lookUpRate(float Value);
    void baseTurnRate(float Value);


public:
	// Sets default values for this character's properties
	AUserCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    void OnTouchBegin(const FVector &NewTouchLocation);




public:
    FTimerHandle timer_handle;

    int     bullet_cap;
    int     score;
    float   health;
    int     rank;

    // Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void setHeal();
    void setSolidColor(const FVector &Color);
    void setScoreAndBulletCap(int Score, int Bullet);
    void applyDamage();
    void giveRank(int Level);
    void setGravityTreshold();

    UFUNCTION(BlueprintCallable,  Category="UserPanel")
    void setUserPanel(class UUserWidget* Panel);
    UFUNCTION(BlueprintCallable,  Category="UserPanel")
    void fireBullet();
	
};
