#include "DELever.h"

#include "DungeonEscapeGameState.h"
#include "Net/UnrealNetwork.h"

ADELever::ADELever()
{
    bReplicates = true;
    SetReplicateMovement(true);

    bActivated = false;
}

void ADELever::Interact_Implementation(AActor* Interactor)
{
    if (!HasAuthority())
    {
        return;
    }

    if (bActivated)
    {
        return;
    }

    bActivated = true;

    if (ADungeonEscapeGameState* GS =
        GetWorld()->GetGameState<ADungeonEscapeGameState>())
    {
        GS->AddActivatedLever();
    }

    OnLeverActivated();

    GEngine->AddOnScreenDebugMessage(
        -1,
        2.f,
        FColor::Yellow,
        TEXT("Lever Activated"));
}

void ADELever::OnRep_Activated()
{
    if (bActivated)
    {
        OnLeverActivated();
    }
}

bool ADELever::IsActivated() const
{
    return bActivated;
}

void ADELever::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ADELever, bActivated);
}