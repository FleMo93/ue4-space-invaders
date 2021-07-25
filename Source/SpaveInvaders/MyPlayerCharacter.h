

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyPlayerCharacter.generated.h"

UCLASS()
class SPAVEINVADERS_API AMyPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RightAreaBound = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LeftAreaBound = -100;
	
	float LeftBound = 0;
	float RightBound = 0;
	float InputAxisValue = 0;
	UCharacterMovementComponent* CharacterMovementComponent;

private:
	void PrintString(FString Text);
	void CheckLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
		Moves the character around the axis value. Only runs on the owner and server.
	*/
	UFUNCTION(BlueprintCallable)
	void MoveByAxis(float AxisValue);
};
