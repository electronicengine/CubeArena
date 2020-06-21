// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuLevelActor.h"
#include "CubeGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AMenuLevelActor::BeginPlay()
{

    setCameraTarget();

    UWidgetBlueprintLibrary::SetInputMode_UIOnly(UGameplayStatics::GetPlayerController(GetWorld(), 0));

    game_instance = Cast<UCubeGameInstance>(GetGameInstance());

    if(game_instance != NULL)
        game_instance->loadMenuWidget();
}
