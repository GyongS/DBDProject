#pragma once
#include "DBDProject.h"
#include "GameFramework/Actor.h"
#include "PalletActor.generated.h"

UCLASS()
class DBDPROJECT_API APalletActor : public AActor
{
	GENERATED_BODY()

public:
	APalletActor();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Box)
		UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, Category = PalletCheckCol)
		USphereComponent* PalletCol1;

	UPROPERTY(EditAnywhere, Category = PalletCheckCol)
		USphereComponent* PalletCol2;

	UPROPERTY(VisibleAnywhere, Category = Box)
		UStaticMeshComponent* Box;
};
