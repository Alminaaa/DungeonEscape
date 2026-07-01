#include "DEPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "DungeonEscapeGameState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ADEPlayerController::ADEPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;

    bEndScreenShown = false;
    CurrentWidget = nullptr;
    TutorialWidget = nullptr;

    MainMenuMapName = TEXT("MainMenuMap");
    TutorialDuration = 8.f;
}

void ADEPlayerController::BeginPlay()
{
    Super::BeginPlay();

    const FString CurrentMapName =
        UGameplayStatics::GetCurrentLevelName(
            GetWorld(),
            true);

    if (CurrentMapName == MainMenuMapName.ToString())
    {
        SetupMainMenu();
    }
    else
    {
        SetupGameplayInput();
        ShowTutorial();
    }
}

void ADEPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    const FString CurrentMapName =
        UGameplayStatics::GetCurrentLevelName(
            GetWorld(),
            true);

    if (CurrentMapName == MainMenuMapName.ToString())
    {
        return;
    }

    if (bEndScreenShown)
    {
        return;
    }

    ADungeonEscapeGameState* GS =
        GetWorld()->GetGameState<ADungeonEscapeGameState>();

    if (!GS)
    {
        return;
    }

    if (GS->IsGameWon())
    {
        ShowVictory();
        return;
    }

    if (GS->IsGameLost())
    {
        ShowDefeat();
    }
}

void ADEPlayerController::SetupMainMenu()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    if (MainMenuWidgetClass)
    {
        CurrentWidget =
            CreateWidget(
                this,
                MainMenuWidgetClass);

        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();

            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(
                CurrentWidget->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(
                EMouseLockMode::DoNotLock);

            SetInputMode(InputMode);
        }
    }
    else
    {
        FInputModeUIOnly InputMode;
        InputMode.SetLockMouseToViewportBehavior(
            EMouseLockMode::DoNotLock);

        SetInputMode(InputMode);
    }
}

void ADEPlayerController::SetupGameplayInput()
{
    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;

    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
}

void ADEPlayerController::ShowTutorial()
{
    if (!IsLocalController())
    {
        return;
    }

    if (!TutorialWidgetClass)
    {
        return;
    }

    TutorialWidget =
        CreateWidget(
            this,
            TutorialWidgetClass);

    if (TutorialWidget)
    {
        TutorialWidget->AddToViewport();

        GetWorldTimerManager().SetTimer(
            TutorialTimerHandle,
            this,
            &ADEPlayerController::HideTutorial,
            TutorialDuration,
            false);
    }
}

void ADEPlayerController::HideTutorial()
{
    if (TutorialWidget)
    {
        TutorialWidget->RemoveFromParent();
        TutorialWidget = nullptr;
    }
}

void ADEPlayerController::ShowVictory()
{
    bEndScreenShown = true;

    SetPause(true);

    bShowMouseCursor = true;

    FInputModeUIOnly InputMode;
    SetInputMode(InputMode);

    if (VictoryWidgetClass)
    {
        CurrentWidget =
            CreateWidget(
                this,
                VictoryWidgetClass);

        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

void ADEPlayerController::ShowDefeat()
{
    bEndScreenShown = true;

    SetPause(true);

    bShowMouseCursor = true;

    FInputModeUIOnly InputMode;
    SetInputMode(InputMode);

    if (DefeatWidgetClass)
    {
        CurrentWidget =
            CreateWidget(
                this,
                DefeatWidgetClass);

        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}