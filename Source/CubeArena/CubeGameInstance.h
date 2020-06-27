// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <vector>
#include "CubeGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class CUBEARENA_API UCubeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	


    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> user_panel_container;

    // Internal reference to the blueprint for gameplay logic
    class UUserWidget* user_panel;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> menu_widget_container;

    // Internal reference to the blueprint for gameplay logic
    class UUserWidget* menu_widget;

    const std::vector<FVector> faces;



    UPROPERTY(EditAnywhere, Category = "Control")
    float light_intensity;

	
protected:

public:
    UCubeGameInstance();

    class UUserWidget* loadMenuWidget();
    class UUserWidget* loadUserPanel();

    const FVector &getface(int FaceId);

    UPROPERTY(EditAnywhere, Category = "Valume")
    float sound_volume;

    UPROPERTY(EditAnywhere, Category = "Control")
    float base_turn_rate;

    UPROPERTY(EditAnywhere, Category = "Control")
    float look_up_rate;

    UFUNCTION(BlueprintCallable, Category = "Control")
    void setTurnRate(float Value);

    UFUNCTION(BlueprintCallable, Category = "Control")
    float getTurnRate();


    UFUNCTION(BlueprintCallable, Category = "SkyLight")
    void setLightIntensity(float Value);

    UFUNCTION(BlueprintCallable, Category = "SkyLight")
    float getLightIntensity();

    UFUNCTION(BlueprintCallable, Category = "Volume")
    void setSoundVolume(float Value);

    UFUNCTION(BlueprintCallable, Category = "Volume")
    float getSoundVolume();



};
