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
    PauseWidget = nullptr;

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

void ADEPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindKey(
            EKeys::Escape,
            IE_Pressed,
            this,
            &ADEPlayerController::TogglePauseMenu);
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
    if (!IsLocalController())
    {
        return;
    }

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
    if (!IsLocalController())
    {
        return;
    }

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
    if (!IsLocalController())
    {
        return;
    }

    if (TutorialWidget)
    {
        TutorialWidget->RemoveFromParent();
        TutorialWidget = nullptr;
    }
}

void ADEPlayerController::TogglePauseMenu()
{
    if (!IsLocalController())
    {
        return;
    }

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

    if (PauseWidget)
    {
        PauseWidget->RemoveFromParent();
        PauseWidget = nullptr;

        SetPause(false);

        bShowMouseCursor = false;
        bEnableClickEvents = false;
        bEnableMouseOverEvents = false;

        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);

        return;
    }

    if (!PauseWidgetClass)
    {
        return;
    }

    PauseWidget =
        CreateWidget(
            this,
            PauseWidgetClass);

    if (PauseWidget)
    {
        PauseWidget->AddToViewport();

        SetPause(true);

        bShowMouseCursor = true;
        bEnableClickEvents = true;
        bEnableMouseOverEvents = true;

        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(
            PauseWidget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(
            EMouseLockMode::DoNotLock);

        SetInputMode(InputMode);
    }
}

void ADEPlayerController::ShowVictory()
{
    if (!IsLocalController())
    {
        return;
    }

    bEndScreenShown = true;

    SetPause(true);

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    FInputModeUIOnly InputMode;
    InputMode.SetLockMouseToViewportBehavior(
        EMouseLockMode::DoNotLock);
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
    if (!IsLocalController())
    {
        return;
    }

    bEndScreenShown = true;

    SetPause(true);

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    FInputModeUIOnly InputMode;
    InputMode.SetLockMouseToViewportBehavior(
        EMouseLockMode::DoNotLock);
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