#pragma once
#include "DBDProject.h"
#include "GameFramework/Actor.h"
#include "Hook_Manager.generated.h"

UCLASS()
class DBDPROJECT_API AHook_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	AHook_Manager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Hook, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* HookMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Hook, meta = (AllowPrivateAccess = "true"))
		USphereComponent* PersonHangCheckCol;

};
