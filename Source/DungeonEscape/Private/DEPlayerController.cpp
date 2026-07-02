#include "DEPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "DungeonEscapeGameState.h"
#include "Kismet/GameplayStatics.h"

ADEPlayerController::ADEPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;

    bEndScreenShown = false;

    CurrentWidget = nullptr;
    TutorialWidget = nullptr;
    HUDWidget = nullptr;
    PauseWidget = nullptr;

    MainMenuMapName = TEXT("MainMenuMap");
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
        ShowHUD();
        ShowTutorial();
    }
}

void ADEPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindKey(
            EKeys::Tab,
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
    if (!IsLocalController() || !TutorialWidgetClass)
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

        bShowMouseCursor = true;
        bEnableClickEvents = true;
        bEnableMouseOverEvents = true;

        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(
            TutorialWidget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(
            EMouseLockMode::DoNotLock);

        SetInputMode(InputMode);

        SetPause(true);
    }
}

void ADEPlayerController::ShowHUD()
{
    if (!IsLocalController() || !HUDWidgetClass)
    {
        return;
    }

    if (HUDWidget)
    {
        return;
    }

    HUDWidget =
        CreateWidget(
            this,
            HUDWidgetClass);

    if (HUDWidget)
    {
        HUDWidget->AddToViewport();
    }
}

void ADEPlayerController::HideHUD()
{
    if (HUDWidget)
    {
        HUDWidget->RemoveFromParent();
        HUDWidget = nullptr;
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

    if (TutorialWidget)
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

    HideHUD();

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

    HideHUD();

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