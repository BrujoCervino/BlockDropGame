// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameStateNotifier.h"
#include "BlockDropPlayerController.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class BLOCKDROPGAME_API ABlockDropPlayerController 
	: public APlayerController, public IGameStateNotifier
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABlockDropPlayerController();

	// Adds to the current score
	// (Non-const parameter, so the score can be manipulated, in case an anomalous value is passed in).
	virtual void AddScore(int32 ScoreToAdd);

	// Returns score
	int32 GetScore() const;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//~ Begin IGameStateNotifier Interface
	virtual void NotifyState(const EGameState::Type State) override;
	//~ End IGameStateNotifier Interface

	// Setter for score.
	void SetScore(const int32 InScore);

	// Updates the displayed score. Currently Blueprint Only.
	UFUNCTION(BlueprintNativeEvent, meta = (BlueprintProtected))
	void UpdateDisplayedScore(const int32 InScore);
	virtual void UpdateDisplayedScore_Implementation(const int32 InScore) {}

	// The widget currently displayed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BlueprintProtected))
	UUserWidget* CurrentWidget;

	// The class of widget with which this class displays the score
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ScoreWidgetClass;

	// The class of widget with which this class displays the score
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PauseWidgetClass;

private:

	// The current score (maybe this should be an unsigned int so it can go further?)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int32 Score;

};
