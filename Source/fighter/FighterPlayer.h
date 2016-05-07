// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "FighterPlayer.generated.h"

UCLASS()
class FIGHTER_API AFighterPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFighterPlayer();

	uint32 Health;
	enum State {Idle, Forward, Backward, Punching, Jumping};
	uint32 PlayerState;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void MoveRight(float Value);
	void Punch();
	virtual void Jump() override;
	
};
