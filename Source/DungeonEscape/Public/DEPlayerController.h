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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> TutorialWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> VictoryWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> DefeatWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    FName MainMenuMapName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    float TutorialDuration;

    UPROPERTY()
    UUserWidget* CurrentWidget;

    UPROPERTY()
    UUserWidget* TutorialWidget;

    FTimerHandle TutorialTimerHandle;

    bool bEndScreenShown;

    void SetupMainMenu();
    void SetupGameplayInput();

    void ShowTutorial();
    void HideTutorial();

    void ShowVictory();
    void ShowDefeat();
};