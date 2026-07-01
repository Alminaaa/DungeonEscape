#include "DEPauseWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

void UDEPauseWidget::ResumeGame()
{
    APlayerController* PC =
        UGameplayStatics::GetPlayerController(
            GetWorld(),
            0);

    if (!PC)
    {
        return;
    }

    RemoveFromParent();

    PC->SetPause(false);
    PC->bShowMouseCursor = false;

    FInputModeGameOnly InputMode;
    PC->SetInputMode(InputMode);
}

void UDEPauseWidget::ReturnToMainMenu()
{
    APlayerController* PC =
        UGameplayStatics::GetPlayerController(
            GetWorld(),
            0);

    if (PC)
    {
        PC->SetPause(false);
    }

    UGameplayStatics::OpenLevel(
        GetWorld(),
        MainMenuMapName);
}

void UDEPauseWidget::QuitGame()
{
    APlayerController* PC =
        UGameplayStatics::GetPlayerController(
            GetWorld(),
            0);

    UKismetSystemLibrary::QuitGame(
        GetWorld(),
        PC,
        EQuitPreference::Quit,
        false);
}