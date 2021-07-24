

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Actor.h"
#include "MyPlayer.generated.h"

UCLASS()
class SPAVEINVADERS_API AMyPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider")
	UShapeComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RightAreaBound = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LeftAreaBound = -100;
	
	float LeftBound = 0;
	float RightBound = 0;
	float InputAxisValue = 0;

private:
	void PrintString(FString Text);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void MoveByAxis(float AxisValue);
};
