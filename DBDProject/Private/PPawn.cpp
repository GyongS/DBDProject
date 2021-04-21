#include "DBDProject.h"
#include "PPawn.h"
#include "..\Public\PPawn.h"

APPawn::APPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APPawn::BeginPlay()
{
	Super::BeginPlay();
}

void APPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void APPawn::PossessedBy(AController* NewCtrl)
{
	Super::PossessedBy(NewCtrl);
}