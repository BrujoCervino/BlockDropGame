// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateNotifier.h"

// Add default functionality here for any IStateNotifier functions that are not pure virtual.

IGameStateNotifier* const IGameStateNotifier::GetActorCastIntoGameStateNotifier(AActor* const ActorToCast) const
{
	return Cast<IGameStateNotifier, AActor>(ActorToCast);
}
