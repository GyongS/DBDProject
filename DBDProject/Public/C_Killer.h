#pragma once
#include "DBDProject.h"
#include "GameFramework/Character.h"
#include "C_Killer.generated.h"

UCLASS(BlueprintType, Blueprintable)
class DBDPROJECT_API AC_Killer : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
		float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
		float AttackRadius;
public:
	AC_Killer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sphere, meta = (AllowPrivateAccess = true))
		USphereComponent* InteractionCol;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sphere, meta = (AllowPrivateAccess = true))
		UBoxComponent* ShoulderCol;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float PersonLifeCount = 3;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class ACCharacter* PersonClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class AHook_Manager* HookClass;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	void Horizontal(float NewAxisValue);
	void Vertical(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void Attack();
	void Interaction();
	

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void StartAttackState();
	void EndAttackState();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool isAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextAttack;	

	UPROPERTY()
		class UKiller_AnimInstance* KillerAnim;

	void DamageCheck();
	void GrabCheck();
	void PickedUpPerson();
	void HangingPerson();
	void HangingStart();
	void HangingEnd();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grab, Meta = (AllowPrivateAccess = true))
		bool isGrabPerson;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grab, Meta = (AllowPrivateAccess = true))
		bool isHangPerson;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grab, Meta = (AllowPrivateAccess = true))
		bool isHookOverlapCheck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isPersonHanging;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool b_KillerView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isEscape = false;
};
