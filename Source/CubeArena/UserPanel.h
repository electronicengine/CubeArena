// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserPanel.generated.h"

/**
 * 
 */
UCLASS()
class CUBEARENA_API UUserPanel : public UUserWidget
{
	GENERATED_BODY()
	

public:

    UFUNCTION(BlueprintImplementableEvent, Category="Health")
    void setHeathPercent(float Percent);

    UFUNCTION(BlueprintImplementableEvent, Category="Fircap")
    void setFireCap(int Cap);

    UFUNCTION(BlueprintImplementableEvent, Category="Score")
    void setScore(float Score);

    UFUNCTION(BlueprintImplementableEvent, Category="Level")
    void setLevel(const FString &Level);

    UFUNCTION(BlueprintImplementableEvent, Category="GameOver")
    void setGameOver(const FString &GameOverStr);


};
