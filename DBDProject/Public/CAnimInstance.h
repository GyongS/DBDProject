#pragma once
#include "DBDProject.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

UCLASS()
class DBDPROJECT_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCAnimInstance();

	UPROPERTY()
		class ACCharacter* PlayerClass;

	UPROPERTY()
		class AEscapeZone* EscapeClass;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayParkourMontage();
	void PlayEscapeMontage();
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsRepair;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInjured;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsProne;		

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Parkour, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ParkourMonstage;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Escape, Meta = (AllowPrivateAccess = true))
		UAnimMontage* EscapeMontage;

	float CurrentMoveSpeed;
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsHanging;
};
