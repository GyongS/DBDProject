#pragma once
#include "DBDProject.h"
#include "GameFramework/Actor.h"
#include "CElectroMachine.generated.h"

UCLASS()
class DBDPROJECT_API ACElectroMachine : public AActor
{
	GENERATED_BODY()

public:
	ACElectroMachine();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Box)
		UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
		UStaticMeshComponent* Box;

	UPROPERTY(VisibleAnywhere, Category = CheckCol)
		USphereComponent* Col1;

	UPROPERTY(VisibleAnywhere, Category = CheckCol)
		USphereComponent* Col2;

	UPROPERTY(VisibleAnywhere, Category = CheckCol)
		USphereComponent* Col3;

	UPROPERTY(VisibleAnywhere, Category = CheckCol)
		USphereComponent* Col4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Generator)
		USkeletalMeshComponent* GeneratorSkelMesh;
};
