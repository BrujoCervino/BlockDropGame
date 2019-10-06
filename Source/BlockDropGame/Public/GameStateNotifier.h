// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BlockDropTypes.h"
#include "GameStateNotifier.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameStateNotifier : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLOCKDROPGAME_API IGameStateNotifier
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	// Lets other actors know the state has changed
	// (Not passing by const reference because EGameState is only one byte large)
	virtual void NotifyState(const EGameState::Type State) = 0;

	// Bad practice: all interfaces should be pure virtual, but that would have resulted in duplication
	// (this function is needed by all actors whhich implement this interface, to notify their owner of changes to the game state).
	// Casts the given actor into an instance of this class.
	// (Maybe use a message bus to avoid unnececessary communication between actors)
	virtual IGameStateNotifier* const GetObjectCastIntoGameStateNotifier(UObject* const ObjectToCast) const;
};