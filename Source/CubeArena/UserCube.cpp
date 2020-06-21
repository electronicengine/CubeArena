// Fill out your copyright notice in the Description page of Project Settings.


#include "UserCube.h"

#include "CubeLevelActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "GameFramework/SpringArmComponent.h"
#include "UserPanel.h"
#include "Kismet/KismetMathLibrary.h"


AUserCube::AUserCube() : solid_color(FMath::RandRange(0.0f, 0.3f),FMath::RandRange(0.0f, 0.2f), FMath::RandRange(0.0f, 0.2))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    user_solid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UserSolid"));
    user_solid->SetupAttachment(RootComponent);

    spring_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    spring_arm->SetupAttachment(RootComponent);

    camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    camera->SetupAttachment(spring_arm);



    base_turn_rate = 0.6f;
    look_up_rate = 0.6f;

    trace_distance = 2000.0f;

    score = 0;
    health = 1.0f;
    bullet_cap = 40;

    middle_of_screen_x = 650.0f;

    gravity_backward_treshold = 0;
    gravity_forward_treshold = 0;

}



// Called when the game starts or when spawned
void AUserCube::BeginPlay()
{
    static FVector TouchLocation;
    Super::BeginPlay();

    level_script = Cast<ACubeLevelActor>(GetWorld()->GetLevelScriptActor());

    user_solid->CreateDynamicMaterialInstance(0);
    user_solid->SetVectorParameterValueOnMaterials(FName("Color"), solid_color);


    player_controller = UGameplayStatics::GetPlayerController(this, 0);

}



// Called every frame
void AUserCube::Tick(float DeltaTime)
{
    static FVector2D old_touch_loc;
    static FVector2D new_touch_loc;
    static ETouchIndex::Type touch_type;
    static bool still_pressed;

    Super::Tick(DeltaTime);


    player_controller->GetInputTouchState(touch_type, new_touch_loc.X, new_touch_loc.Y, still_pressed);

    if(still_pressed == true && touch_type == ETouchIndex::Touch1)
    {


        if((UKismetMathLibrary::Abs(new_touch_loc.X - old_touch_loc.X) <= 28) &&
                (UKismetMathLibrary::Abs(new_touch_loc.Y - old_touch_loc.Y) <= 28))
        {
            this->AddControllerYawInput((new_touch_loc.X - old_touch_loc.X) * (base_turn_rate));
            this->AddControllerPitchInput((new_touch_loc.Y - old_touch_loc.Y) *(look_up_rate));
        }

        old_touch_loc = new_touch_loc;

    }

}



// Called to bind functionality to input
void AUserCube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

//    PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AUserCube::OnTouchBegin);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUserCube::fireBullet);

    PlayerInputComponent->BindAxis("MoveForward", this, &AUserCube::moveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AUserCube::moveRight);

    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AUserCube::baseTurnRate);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AUserCube::lookUpRate);

}



void AUserCube::setSolidColor(const FVector &Color)
{
    solid_color = Color;

    user_solid->SetVectorParameterValueOnMaterials(FName("Color"), solid_color);

}



void AUserCube::setScoreAndBulletCap(int Score, int Bullet)
{
    score +=Score;
    bullet_cap += Bullet;

    if(user_panel != NULL)
    {
        Cast<UUserPanel>(user_panel)->setFireCap(bullet_cap);
        Cast<UUserPanel>(user_panel)->setScore(score);
    }

}



void AUserCube::applyDamage()
{
    health -= 0.1;

    if(health > 0.0f)
    {
        if(user_panel != NULL)
            Cast<UUserPanel>(user_panel)->setHeathPercent(health);
    }
    else if(health <= 0.0f)
    {
        if(user_panel != NULL)
            Cast<UUserPanel>(user_panel)->setHeathPercent(health);

        if(user_panel != NULL)
            Cast<UUserPanel>(user_panel)->setGameOver(FString("You Died! Score: ") + FString::FromInt(score));
        Destroy();
//        destructable_component->ApplyRadiusDamage(20.f, GetActorLocation(), 20.0f, 20.0f, true);


//        FGenericPlatformMisc::RequestExit(false);
//        GetWorld()->GetTimerManager().SetTimer(timer_handle_destroy, this, &AAICube::eraseAI, 2.0f, false);
    }
}



void AUserCube::giveRank(int Level)
{
    if(user_panel != NULL)
        Cast<UUserPanel>(user_panel)->setLevel(FString("Rank: ") + FString::FromInt(Level));
}



void AUserCube::setGravityTreshold()
{
    float input;

    for(int i = 0; i<5000; i++)
        input = GetInputAxisValue(FName("moveForward"));

    gravity_backward_treshold = input - 0.5f;
    gravity_forward_treshold = input + 0.5f;

//    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::SanitizeFloat(gravity_forward_treshold));

}



void AUserCube::setUserPanel(UUserWidget *Panel)
{
    user_panel = Panel;

    if(user_panel != NULL)
    {
        Cast<UUserPanel>(user_panel)->setHeathPercent(health);
        Cast<UUserPanel>(user_panel)->setFireCap(bullet_cap);
        Cast<UUserPanel>(user_panel)->setScore(score);
    }
}



void AUserCube::moveForward(float Value)
{

    if(Value > gravity_forward_treshold)
        Value = 1.0f;
    else if(Value < gravity_backward_treshold)
        Value = -1.0f;
    else
        Value = 0.0f;

    if((Controller) && Value != 0.0f)
    {

        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X);


        AddMovementInput(direction, Value);

    }


}



void AUserCube::moveRight(float Value)
{
    if((Controller) && Value != 0.0f)
    {

        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y);

        AddMovementInput(direction, Value);
    }
}



void AUserCube::lookUpRate(float Value)
{
    AddControllerPitchInput((Value * look_up_rate * (float)(GetWorld()->GetDeltaSeconds())));

}



void AUserCube::baseTurnRate(float Value)
{
    AddControllerYawInput(Value * base_turn_rate * (float)(GetWorld()->GetDeltaSeconds()));
}



void AUserCube::fireBullet()
{

    ABullet *bullet_ptr;
    if (bullet_container != NULL && bullet_cap > 0)
    {

        UWorld* const World = GetWorld();
        if (World != NULL)
        {

            FRotator spawn_rotation;
            FVector spawn_location;

            GetController()->GetPlayerViewPoint(spawn_location, spawn_rotation);
//            spawn_rotation= GetViewRotation();
            spawn_rotation = GetActorRotation();
            spawn_location = GetActorLocation() + (spawn_rotation.Vector() * 100.0f) ;

            spawn_rotation.Pitch += 2.0f;

            bullet_ptr = World->SpawnActor<ABullet>(bullet_container,  spawn_location, spawn_rotation);

            bullet_ptr->setSolidColor(solid_color);
            bullet_ptr->setBulletOwner(this);

            bullet_cap--;

            if(user_panel != NULL)
                Cast<UUserPanel>(user_panel)->setFireCap(bullet_cap);

        }
    }
    else if(bullet_cap == 0)
    {
        if(user_panel != NULL)
            Cast<UUserPanel>(user_panel)->setGameOver(FString("No bullet! Score: ") + FString::FromInt(score));
        Destroy();
    }
}


void AUserCube::setHeal()
{
    health = 1.0f;

    if(user_panel != NULL)
        Cast<UUserPanel>(user_panel)->setHeathPercent(health);

}
