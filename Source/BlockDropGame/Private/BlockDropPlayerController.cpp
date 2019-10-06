// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDropPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "BlockDropPawn.h"			// This needs to be removed, due to bad code design: why is the player controller allowed to access this?
#include "BlockDropGameModeBase.h"	// This needs to be removed, due to bad code design: why is the player controller allowed to access this?

ABlockDropPlayerController::ABlockDropPlayerController()
	:
	CurrentWidget(nullptr),
	ScoreWidgetClass(UUserWidget::StaticClass()),
	Score(0)
{
}

void ABlockDropPlayerController::AddScore(int32 ScoreToAdd)
{
	// Do the math

	// Set the score
	Score += ScoreToAdd;
	// Update the displayed score
	NotifyState(EGameState::EGS_Scored);
}

int32 ABlockDropPlayerController::GetScore() const
{
	return Score;
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
	switch (State)
	{
		case(EGameState::EGS_GameOver):
		{
			break;
		}
		case(EGameState::EGS_Scored):
		{
			int32 ScoreToDisplay = 0;
			if (ABlockDropGameModeBase* const GM = Cast<ABlockDropGameModeBase, AActor>(GetOwner()->GetOwner()))
			{
				ScoreToDisplay = GM->GetScorePerGameState(State);
			}
			
			UpdateDisplayedScore(ScoreToDisplay);
			UpdateDisplayedScore_Implementation(ScoreToDisplay);
			break;
		}
	}
}

void ABlockDropPlayerController::SetScore(const int32 InScore)
{
}
