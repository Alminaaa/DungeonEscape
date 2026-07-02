#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DEPlayerController.generated.h"

class UUserWidget;

UCLASS()
class DUNGEONESCAPE_API ADEPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ADEPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupInputComponent() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> TutorialWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> HUDWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> VictoryWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> DefeatWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> PauseWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    FName MainMenuMapName;

    UPROPERTY()
    UUserWidget* CurrentWidget;

    UPROPERTY()
    UUserWidget* TutorialWidget;

    UPROPERTY()
    UUserWidget* HUDWidget;

    UPROPERTY()
    UUserWidget* PauseWidget;

    bool bEndScreenShown;

    void SetupMainMenu();
    void SetupGameplayInput();

    void ShowTutorial();
    void ShowHUD();
    void HideHUD();

    void TogglePauseMenu();

    void ShowVictory();
    void ShowDefeat();
};