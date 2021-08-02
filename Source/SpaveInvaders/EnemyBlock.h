

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"
#include "Containers/Array.h"
#include "Net/UnrealNetwork.h"
#include "BaseEnemy.h"
#include "EnemyBlock.generated.h"

UENUM(BlueprintType)
enum MovementDirection
{
	Left = 0 UMETA(DisplayName = "Left"),
	Right = 1 UMETA(DUsplaceName = "Right"),
};

UCLASS()
class SPAVEINVADERS_API AEnemyBlock : public ACharacter
{
	GENERATED_BODY()

private:
	TArray<ABaseEnemy *> Enemies;
	bool Alive = false;
	UWorld *MyWorld;
	FScriptDelegate MyOnEnemyDestroyed;
	TEnumAsByte<MovementDirection> CurrentMovementDirection;
	float CurrentMovementSpeed = MovementSpeed;

public:
	AEnemyBlock();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Behaviour")
	float LeftBound = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Behaviour")
	float RightBound = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	float MovementSpeed = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	float MovementSpeedIncrease = 50;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Behaviour")
	TEnumAsByte<MovementDirection> InitialMovementDirection;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Behaviour")
	float RowHeightMovement = 100;

private:
	UFUNCTION(Server, Unreliable)
	void MoveBlock();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnEnemyDestroyed(ABaseEnemy *Enemy);
};
