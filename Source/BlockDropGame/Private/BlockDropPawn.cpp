// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDropPawn.h"				
#include "BlockDropPlayerController.h"	
#include "BlockDropper.h"			
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"	
#include "EngineUtils.h"			
#include "Kismet/GameplayStatics.h"	
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABlockDropPawn::ABlockDropPawn()
	: 
	BlockDropper(nullptr),
	Camera(CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"))),
	bPlayerRequestedQuitGame(false)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	// Let the camera be the pivot of this pawn
	SetRootComponent(Camera);
}

// Called when the game starts or when spawned
void ABlockDropPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// There should only be one block dropper instance in the world, so get it and store it.
	for (TActorIterator<ABlockDropper> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		BlockDropper = *ActorItr;
		break;
	}
	
	GetController()->SetOwner(this);
}

// Called every frame
void ABlockDropPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ABlockDropPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// In-game actions & axes
	PlayerInputComponent->BindAction("ReleaseBlock", IE_Pressed, this, &ABlockDropPawn::ReleaseBlock);

	// UI-based actions & axes
	PlayerInputComponent->BindAction("PauseGame", IE_Pressed, this, &ABlockDropPawn::PauseGame).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("QuitGame", IE_Pressed, this, &ABlockDropPawn::QuitGame);

}

void ABlockDropPawn::SetBlockDropper(ABlockDropper* InBlockDopper)
{
	BlockDropper = InBlockDopper;
}

void ABlockDropPawn::ReleaseBlock()
{
	if (BlockDropper)
	{
		BlockDropper->ReleaseBlock();
	}
}

void ABlockDropPawn::PauseGame()
{
	// Store whether we should pause or unpause the game
	const bool bShouldPause = !(UGameplayStatics::IsGamePaused(this));

	// Set the visibility of the pause widget
	if (ABlockDropPlayerController* const PC = Cast<ABlockDropPlayerController, AController>( GetController() ))
	{
		PC->OnPaused(bShouldPause);
	}

	// Set the game's pause state
	UGameplayStatics::SetGamePaused(this, bShouldPause);
}

bool ABlockDropPawn::RequestQuitGame()
{
	// If the player hasn't pressed the quit button yet, 
	if (!bPlayerRequestedQuitGame)
	{
		// Ask the player whether they definitely want to quit
		UKismetSystemLibrary::PrintString(this, FString("ART THOU SURE THOU WILST THE GAME TO END!?"));
		// If this function is entered again, quit the game.
		bPlayerRequestedQuitGame = true;

		return false;
	}
	// Elsewise, the player wants to quit the game
	return true;
}

void ABlockDropPawn::QuitGame()
{
	// If the player wants to quit the game,
	if( RequestQuitGame() )
	{
		// Quit the game
		FGenericPlatformMisc::RequestExit(false);
	}
}

void ABlockDropPawn::NotifyState(const EGameState::Type State)
{
	switch (State)
	{
		case(EGameState::EGS_GameOver):
		{
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
	// Tell the game mode we scored
	if (IGameStateNotifier* const GameModeNotifier = Cast<IGameStateNotifier, AActor>(GetOwner()))
	{
		GameModeNotifier->NotifyState(State);
	}
	// Tell the our controller we scored
	if (IGameStateNotifier* const ControllerNotifier = Cast<IGameStateNotifier, AActor>(GetController()))
	{
		ControllerNotifier->NotifyState(State);
	}
}

ABlockDropper * ABlockDropPawn::GetBlockDropper() const
{
	return BlockDropper;
}

