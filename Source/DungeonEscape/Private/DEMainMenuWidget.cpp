#include "DEMainMenuWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

void UDEMainMenuWidget::HostGame()
{
    if (GameplayMapName.IsEmpty())
    {
        return;
    }

    UGameplayStatics::OpenLevel(
        GetWorld(),
        FName(*GameplayMapName),
        true,
        TEXT("listen"));
}

void UDEMainMenuWidget::JoinGameByIP(const FString& IPAddress)
{
    if (IPAddress.IsEmpty())
    {
        return;
    }

    APlayerController* PC =
        UGameplayStatics::GetPlayerController(
            GetWorld(),
            0);

    if (!PC)
    {
        return;
    }

    PC->ClientTravel(
        IPAddress,
        TRAVEL_Absolute);
}

void UDEMainMenuWidget::QuitGame()
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