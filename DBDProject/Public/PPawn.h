#pragma once
#include "DBDProject.h"
#include "GameFramework/Pawn.h"
#include "PPawn.generated.h"

UCLASS()
class DBDPROJECT_API APPawn : public APawn
{
	GENERATED_BODY()

public:
	APPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewCtrl) override;
};
