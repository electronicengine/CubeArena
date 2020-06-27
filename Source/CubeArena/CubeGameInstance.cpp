// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeGameInstance.h"
#include "Engine/Engine.h"
#include "UserPanel.h"
#include "MainMenu.h"



UCubeGameInstance::UCubeGameInstance() : faces{FVector(0.0f, 0.0f, 0.0f),
                                               FVector(0.0f, 0.2f, 0.0f),
                                               FVector(0.0f, 0.4f, 0.0f),
                                               FVector(0.0f, 0.6f, 0.0f),
                                               FVector(0.0f, 0.8f, 0.0f),
                                               FVector(0.2f, 0.0f, 0.0f),
                                               FVector(0.2f, 0.2f, 0.0f),
                                               FVector(0.2f, 0.4f, 0.0f),
                                               FVector(0.2f, 0.6f, 0.0f),
                                               FVector(0.2f, 0.8f, 0.0f),
                                               FVector(0.4f, 0.0f, 0.0f),
                                               FVector(0.4f, 0.2f, 0.0f),
                                               FVector(0.4f, 0.4f, 0.0f),
                                               FVector(0.4f, 0.6f, 0.0f),
                                               FVector(0.4f, 0.8f, 0.0f),
                                               FVector(0.6f, 0.0f, 0.0f),
                                               FVector(0.6f, 0.2f, 0.0f),
                                               FVector(0.6f, 0.4f, 0.0f),
                                               FVector(0.6f, 0.6f, 0.0f),
                                               FVector(0.6f, 0.8f, 0.0f),
                                               FVector(0.8f, 0.0f, 0.0f),
                                               FVector(0.8f, 0.2f, 0.0f),
                                               FVector(0.8f, 0.4f, 0.0f),
                                               FVector(0.8f, 0.6f, 0.0f),
                                               FVector(0.8f, 0.8f, 0.0f)}
{

    base_turn_rate = 0.6f;
    look_up_rate = 0.6f;
    light_intensity = 3.0f;
    sound_volume = 0.75f;


}

UUserWidget* UCubeGameInstance::loadMenuWidget()
{
    menu_widget = CreateWidget<UUserWidget>(this, menu_widget_container);

    if(menu_widget != NULL)
        menu_widget->AddToViewport();

    return menu_widget;
}



UUserWidget* UCubeGameInstance::loadUserPanel()
{
//    menu_widget->RemoveFromParent();

    user_panel = CreateWidget<UUserWidget>(this, user_panel_container);

    if(user_panel != NULL)
        user_panel->AddToViewport();

    return user_panel;
}



const FVector &UCubeGameInstance::getface(int FaceId)
{
    if(FaceId > 24)
        FaceId = 24;

    return faces[FaceId];

}



void UCubeGameInstance::setTurnRate(float Value)
{
    base_turn_rate = Value;
    look_up_rate = Value;
}



float UCubeGameInstance::getTurnRate()
{
    return base_turn_rate;
}



void UCubeGameInstance::setLightIntensity(float Value)
{
    light_intensity = Value;
}



float UCubeGameInstance::getLightIntensity()
{
    return light_intensity;
}



void UCubeGameInstance::setSoundVolume(float Value)
{
    sound_volume = Value;
}



float UCubeGameInstance::getSoundVolume()
{
    return sound_volume;
}
