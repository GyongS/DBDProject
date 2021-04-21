#pragma once
#include "DBDProject.h"
#include "GameFramework/Character.h"
#include "EscapeZone.generated.h"

UCLASS()
class DBDPROJECT_API AEscapeZone : public ACharacter
{
	GENERATED_BODY()
	
public:	
	AEscapeZone();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EscapeZone, Meta = (AllowPrivateAccess = true))
		UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EscapeZone, Meta = (AllowPrivateAccess = true))
		USkeletalMeshComponent* EscapeZoneMesh;
	
private:	
	UPROPERTY()
		class ACCharacter* PlayerClass;

	UPROPERTY()
		UAnimInstance* AnimInstance;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EscapeZone, Meta = (AllowprivateAccess = true))
		bool OpenEscapeZone;


};
