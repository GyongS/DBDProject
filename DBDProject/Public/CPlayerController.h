#pragma once
#include "DBDProject.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

UCLASS()
class DBDPROJECT_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
};
