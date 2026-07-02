#include "DETutorialWidget.h"

#include "GameFramework/PlayerController.h"

void UDETutorialWidget::CloseTutorial()
{
    RemoveFromParent();

    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetPause(false);

        PC->bShowMouseCursor = false;
        PC->bEnableClickEvents = false;
        PC->bEnableMouseOverEvents = false;

        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }
}