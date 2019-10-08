// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

namespace EGameState
{
	enum Type : uint8
	{
		EGS_GameOver,
		EGS_Scored,
		EGS_CollectedCollectable,
		EGS_CollectedSpecialCollectable,
		EGS_MissedCollectable
	};
}
