#include "DBDProject.h"
#include "CPlayerController.h"
#include "..\Public\CPlayerController.h"

void ACPlayerController::PostInitializeComponents()
{
	ABLOG_S(Warning);
	Super::PostInitializeComponents();
}

void ACPlayerController::OnPossess(APawn* aPawn)
{
	ABLOG_S(Warning);
	Super::OnPossess(aPawn);
}