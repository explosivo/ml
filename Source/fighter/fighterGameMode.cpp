// Fill out your copyright notice in the Description page of Project Settings.

#include "fighter.h"
#include "fighterGameMode.h"

AfighterGameMode::AfighterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
