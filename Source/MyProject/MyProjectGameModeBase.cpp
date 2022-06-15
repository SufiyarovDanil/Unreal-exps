// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectGameModeBase.h"
#include "MyCharacter.h"

AMyProjectGameModeBase::AMyProjectGameModeBase() {
	DefaultPawnClass = AMyCharacter::StaticClass();
}