// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDropPawn.h"
#include "BlockDropper.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EngineUtils.h"

// Sets default values
ABlockDropPawn::ABlockDropPawn()
	: 
	Camera(CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")))
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

	PlayerInputComponent->BindAction("ReleaseBlock", IE_Pressed, this, &ABlockDropPawn::ReleaseBlock);
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

void ABlockDropPawn::NotifyState(const EGameState::Type State)
{
	switch (State)
	{
		case(EGameState::EGS_GameOver):
		{
			break;
		}
		case(EGameState::EGS_Scored):
		{
			break;
		}
	}
	if (IGameStateNotifier* const Notifier = Cast<IGameStateNotifier, AActor>(GetOwner()))
	{
		Notifier->NotifyState(State);
	}
}

ABlockDropper * ABlockDropPawn::GetBlockDropper() const
{
	return BlockDropper;
}

