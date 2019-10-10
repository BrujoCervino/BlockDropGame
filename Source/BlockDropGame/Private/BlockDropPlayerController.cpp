// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDropPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "BlockDropGameModeBase.h"	// This needs to be removed, due to bad code design: why is the player controller allowed to access this?

ABlockDropPlayerController::ABlockDropPlayerController()
	:
	CurrentWidget(nullptr),
	ScoreWidgetClass(UUserWidget::StaticClass()),
	PauseMenuWidgetClass(UUserWidget::StaticClass()),
	GameOverMenuClass(UUserWidget::StaticClass()),
	Score(0)
{
}

void ABlockDropPlayerController::AddScore(int32 ScoreToAdd)
{
	// Set the score
	Score += ScoreToAdd;
	// Update the displayed score
	NotifyState(EGameState::EGS_PlacedCommonBlock);
}

int32 ABlockDropPlayerController::GetScore() const
{
	return Score;
}

void ABlockDropPlayerController::OnPaused(const bool bPaused)
{
	// If the current widget is valid,
	if (CurrentWidget)
	{
		// Remove and nullify it.
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	
	// Determine the new widget class: if the game was paused, the pause widget. Else, the default score widget
	UClass* const NewWidgetClass = bPaused ? PauseMenuWidgetClass : ScoreWidgetClass;

	// If the new widget class is valid,
	if (NewWidgetClass)
	{
		// Create it,
		CurrentWidget = CreateWidget<UUserWidget>(this, NewWidgetClass);
		// Ensure it's valid,
		if (CurrentWidget)
		{
			// Add it to the viewport.
			CurrentWidget->AddToViewport();
		}
	}
}

void ABlockDropPlayerController::RestartLevel()
{
	// Show the game over screen
	
	// Restart the level
	Super::RestartLevel();
}

void ABlockDropPlayerController::BeginPlay()
{
	CurrentWidget = CreateWidget<UUserWidget>(this, ScoreWidgetClass);
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport();
	}
}

void ABlockDropPlayerController::NotifyState(const EGameState::Type State)
{
	int32 ScoreToDisplay = 0;
	switch (State)
	{
		case(EGameState::EGS_GameOver):
		{
			RestartLevel();
			break;
		}
		case(EGameState::EGS_PlacedCommonBlock):
		{
			break;
		}
		case(EGameState::EGS_CollectedCommonCollectable):
		{
			break;
		}
		case(EGameState::EGS_CollectedRareCollectable):
		{
			break;
		}
		case(EGameState::EGS_MissedCollectable):
		{
			break;
		}
		default:
		{
			// This should remain unreachable
			checkNoEntry();
			break;
		}
	}
	// This can be avoided by just having an interface call or event sent to this class by the pawn,
	//		recieved by the game mode.
	if (ABlockDropGameModeBase* const GM = Cast<ABlockDropGameModeBase, AActor>(GetOwner()->GetOwner()))
	{
		ScoreToDisplay = GM->GetScorePerGameState(State);
	}

	UpdateDisplayedScore(ScoreToDisplay);
	UpdateDisplayedScore_Implementation(ScoreToDisplay);
}

void ABlockDropPlayerController::SetScore(const int32 InScore)
{
}

void ABlockDropPlayerController::UpdateDisplayedScore_Implementation(const int32 InScore)
{
	UpdateDisplayedScore(Score);
}
