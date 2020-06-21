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

	
protected:

public:
    UCubeGameInstance();

    class UUserWidget* loadMenuWidget();
    class UUserWidget* loadUserPanel();

    const FVector &getface(int FaceId);

};
