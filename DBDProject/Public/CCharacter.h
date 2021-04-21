#pragma once
#include "DBDProject.h"
#include "GameFramework/Character.h"
#include "CCharacter.generated.h"

UENUM(BlueprintType)
enum class ECurrentState : uint8
{
	Health = 0,
	Injured,
	Dying,
	Dead
};

UCLASS(BlueprintType, Blueprintable)
class DBDPROJECT_API ACCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACCharacter();
	
protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ECurrentState _PersonState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* ServeCamera;

	UPROPERTY(VisibleAnywhere, Category = Collision)
		USphereComponent* InteractionSphere;

	/*UPROPERTY(VisibleAnywhere, Category = Collision)
		UCapsuleComponent* PalletCheckCollision;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class ACElectroMachine* ElectroMachine;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class APalletActor* PalletClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class AEscapeZone* EscapeZoneClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class AC_Killer* KillerClass;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool b_Repair = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool b_EscapeZone = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool b_CheckPallet = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isGrab = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isHang = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float LifeCount = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool b_MachineOverlapCheck = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int MinMachineGauge = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int MaxMachineGauge = 1000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int GeneratorCount = 1;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		
private:
	UPROPERTY()
		class UCAnimInstance* PersonAnimInstance;

	UPROPERTY()
		class UEscapeZoneAnimInstance* EscapeZoneAnimInstance;

	UFUNCTION()
		void OnPersonMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Parkour, Meta = (AllowPrivateAccess = true))
		bool b_Parkour = false;



private:
	bool b_Running = false;	
	void Horizontal(float NewAxisValue);
	void Vertical(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LeftShift();
	void Crouching();
	void Interaction();
	void ParKour();
};
