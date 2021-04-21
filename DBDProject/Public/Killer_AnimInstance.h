#pragma once
#include "DBDProject.h"
#include "Animation/AnimInstance.h"
#include "Killer_AnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnGrabisPersonCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnPickUpCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHangingCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHangingStartDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHangingEndDelegate);

UCLASS()
class DBDPROJECT_API UKiller_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UKiller_AnimInstance();

	UPROPERTY()
		class AC_Killer* KillerClass;

	UPROPERTY()
		class ACCharacter* PersonClass;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();

private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck();

	UFUNCTION()
		void AnimNotify_isGrabPerson();
	
	UFUNCTION()
		void AnimNotify_PickUpCheck();

	UFUNCTION()
		void AnimNotify_HangingCheck();

	UFUNCTION()
		void AnimNotify_HangingEnd();

	UFUNCTION()
		void AnimNotify_HangingStart();
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = "true"))
		float CurrentPawnSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grab, Meta = (AllowPrivateAccess = "true"))
		bool isGrab;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grab, Meta = (AllowPrivateAccess = "true"))
		bool isHang;

public:
	FOnAttackHitCheckDelegate OnAttackHitCheck;            
	FOnGrabisPersonCheckDelegate OnGrabisPersonCheck;
	FOnPickUpCheckDelegate OnPickUpCheck;
	FOnHangingCheckDelegate OnHangingCheck;
	FOnHangingStartDelegate OnHangingStart;
	FOnHangingEndDelegate OnHangingEnd;

};