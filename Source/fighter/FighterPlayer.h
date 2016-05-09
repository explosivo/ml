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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FighterPlayer")
	int32 TotalHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FighterPlayer")
	int32 Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FighterPlayer")
	int32 PunchDamage;
	UENUM(BlueprintType)
	enum State {Idle, Forward, Backward, Punching, Jumping};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FighterPlayer")
	int32 FighterPlayerState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FighterPlayer")
	bool bIsPunching;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FighterPlayer")
	bool bIsPlayer1;
	bool bMovementPressed;

	void LogData();

	UFUNCTION(BlueprintCallable, Category = "FighterPlayer")
	void MoveRight(float Value);
	UFUNCTION(BlueprintImplementableEvent, Category = "FighterPlayer")
	void PunchEvent();
	UFUNCTION(BlueprintCallable, Category = "FighterPlayer")
	void Punch();
	UFUNCTION(BlueprintCallable, Category = "FighterPlayer")
	void TakeDamage(int32 Damage);
	UFUNCTION(BlueprintCallable, Category = "FighterPlayer")
	bool IsDead();
	virtual void Jump() override;
	
};
