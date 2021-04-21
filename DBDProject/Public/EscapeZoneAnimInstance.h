#pragma once
#include "DBDProject.h"
#include "Animation/AnimInstance.h"
#include "EscapeZoneAnimInstance.generated.h"

UCLASS()
class DBDPROJECT_API UEscapeZoneAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEscapeZoneAnimInstance();

	UPROPERTY()
		class AEscapeZone* Escape_Class;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowprivateAccess = true))
		bool IsOpen;
};
