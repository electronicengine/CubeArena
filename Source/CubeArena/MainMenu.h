// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserPanel.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class CUBEARENA_API UMainMenu : public UUserPanel
{
	GENERATED_BODY()
	
    class UCubeGameInstance *game_instance;

protected:

    UFUNCTION(BlueprintCallable, Category="Start")
    void startButtonPressed();

public:

	
};
