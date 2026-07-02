#include "DEGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

UDEGameInstance::UDEGameInstance()
{
    SessionName = NAME_GameSession;
    GameplayMapName = TEXT("Lvl_DungeonEscape");
}

void UDEGameInstance::Init()
{
    Super::Init();

    IOnlineSubsystem* OnlineSubsystem =
        IOnlineSubsystem::Get();

    if (OnlineSubsystem)
    {
        SessionInterface =
            OnlineSubsystem->GetSessionInterface();

        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Online Subsystem: %s"),
            *OnlineSubsystem->GetSubsystemName().ToString());
    }
    else
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("No Online Subsystem found"));
    }
}

void UDEGameInstance::HostSession()
{
    if (!SessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("SessionInterface invalid"));
        return;
    }

    if (SessionInterface->GetNamedSession(SessionName))
    {
        DestroyCurrentSession();
        return;
    }

    FOnlineSessionSettings SessionSettings;

    IOnlineSubsystem* OnlineSubsystem =
        IOnlineSubsystem::Get();

    const bool bIsSteam =
        OnlineSubsystem &&
        OnlineSubsystem->GetSubsystemName() == TEXT("STEAM");

    SessionSettings.bIsLANMatch = !bIsSteam;
    SessionSettings.NumPublicConnections = 2;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bAllowJoinViaPresence = true;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.bUseLobbiesIfAvailable = true;

    SessionSettings.Set(
        FName("DungeonEscape"),
        FString("DungeonEscapeSession"),
        EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    CreateSessionCompleteDelegateHandle =
        SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
            FOnCreateSessionCompleteDelegate::CreateUObject(
                this,
                &UDEGameInstance::OnCreateSessionComplete));

    const ULocalPlayer* LocalPlayer =
        GetFirstGamePlayer();

    if (!LocalPlayer)
    {
        UE_LOG(LogTemp, Error, TEXT("No local player"));
        return;
    }

    const bool bSuccess =
        SessionInterface->CreateSession(
            *LocalPlayer->GetPreferredUniqueNetId(),
            SessionName,
            SessionSettings);

    if (!bSuccess)
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(
            CreateSessionCompleteDelegateHandle);

        UE_LOG(LogTemp, Error, TEXT("CreateSession failed to start"));
    }
}

void UDEGameInstance::OnCreateSessionComplete(
    FName CreatedSessionName,
    bool bWasSuccessful)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(
            CreateSessionCompleteDelegateHandle);
    }

    if (!bWasSuccessful)
    {
        UE_LOG(LogTemp, Error, TEXT("CreateSession failed"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("CreateSession success"));

    UGameplayStatics::OpenLevel(
        GetWorld(),
        FName(*GameplayMapName),
        true,
        TEXT("listen"));
}

void UDEGameInstance::FindAndJoinSession()
{
    if (!SessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("SessionInterface invalid"));
        return;
    }

    SessionSearch =
        MakeShareable(new FOnlineSessionSearch());

    IOnlineSubsystem* OnlineSubsystem =
        IOnlineSubsystem::Get();

    const bool bIsSteam =
        OnlineSubsystem &&
        OnlineSubsystem->GetSubsystemName() == TEXT("STEAM");

    SessionSearch->bIsLanQuery = !bIsSteam;
    SessionSearch->MaxSearchResults = 20;
    SessionSearch->PingBucketSize = 50;

    FindSessionsCompleteDelegateHandle =
        SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
            FOnFindSessionsCompleteDelegate::CreateUObject(
                this,
                &UDEGameInstance::OnFindSessionsComplete));

    const ULocalPlayer* LocalPlayer =
        GetFirstGamePlayer();

    if (!LocalPlayer)
    {
        UE_LOG(LogTemp, Error, TEXT("No local player"));
        return;
    }

    const bool bSuccess =
        SessionInterface->FindSessions(
            *LocalPlayer->GetPreferredUniqueNetId(),
            SessionSearch.ToSharedRef());

    if (!bSuccess)
    {
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(
            FindSessionsCompleteDelegateHandle);

        UE_LOG(LogTemp, Error, TEXT("FindSessions failed to start"));
    }
}

void UDEGameInstance::OnFindSessionsComplete(
    bool bWasSuccessful)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(
            FindSessionsCompleteDelegateHandle);
    }

    if (!bWasSuccessful || !SessionSearch.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("FindSessions failed"));
        return;
    }

    if (SessionSearch->SearchResults.Num() <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No sessions found"));
        return;
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Found %d sessions"),
        SessionSearch->SearchResults.Num());

    JoinSessionCompleteDelegateHandle =
        SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
            FOnJoinSessionCompleteDelegate::CreateUObject(
                this,
                &UDEGameInstance::OnJoinSessionComplete));

    const ULocalPlayer* LocalPlayer =
        GetFirstGamePlayer();

    if (!LocalPlayer)
    {
        UE_LOG(LogTemp, Error, TEXT("No local player"));
        return;
    }

    SessionInterface->JoinSession(
        *LocalPlayer->GetPreferredUniqueNetId(),
        SessionName,
        SessionSearch->SearchResults[0]);
}

void UDEGameInstance::OnJoinSessionComplete(
    FName JoinedSessionName,
    EOnJoinSessionCompleteResult::Type Result)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(
            JoinSessionCompleteDelegateHandle);
    }

    if (Result != EOnJoinSessionCompleteResult::Success)
    {
        UE_LOG(LogTemp, Error, TEXT("JoinSession failed"));
        return;
    }

    FString ConnectString;

    if (!SessionInterface->GetResolvedConnectString(
        JoinedSessionName,
        ConnectString))
    {
        UE_LOG(LogTemp, Error, TEXT("Could not get connect string"));
        return;
    }

    APlayerController* PC =
        UGameplayStatics::GetPlayerController(
            GetWorld(),
            0);

    if (PC)
    {
        PC->ClientTravel(
            ConnectString,
            TRAVEL_Absolute);
    }
}

void UDEGameInstance::DestroyCurrentSession()
{
    if (!SessionInterface.IsValid())
    {
        return;
    }

    DestroySessionCompleteDelegateHandle =
        SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
            FOnDestroySessionCompleteDelegate::CreateUObject(
                this,
                &UDEGameInstance::OnDestroySessionComplete));

    SessionInterface->DestroySession(SessionName);
}

void UDEGameInstance::OnDestroySessionComplete(
    FName DestroyedSessionName,
    bool bWasSuccessful)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(
            DestroySessionCompleteDelegateHandle);
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("DestroySession complete: %s"),
        bWasSuccessful ? TEXT("Success") : TEXT("Failed"));
}