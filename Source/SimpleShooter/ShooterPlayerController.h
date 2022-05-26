// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

		virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

		UFUNCTION(BlueprintPure)
			bool GetPlayerWin() const;

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> HUDClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> LoseScreenClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> WinScreenClass;

	UPROPERTY()
		UUserWidget* HUD;

	UPROPERTY(EditAnywhere)
		float RestartDelay = 5.f;

	UPROPERTY()
		bool PlayerWin = false;

	FTimerHandle RestartTimer;
	
};
