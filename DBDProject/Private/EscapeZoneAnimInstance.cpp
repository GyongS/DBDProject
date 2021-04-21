#include "DBDProject.h"
#include "EscapeZoneAnimInstance.h"
#include "EscapeZone.h"

UEscapeZoneAnimInstance::UEscapeZoneAnimInstance()
{
	IsOpen = false;
}

void UEscapeZoneAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();

	if (::IsValid(Pawn))
	{
		Escape_Class = Cast<AEscapeZone>(Pawn);

		if (Escape_Class)
		{
			IsOpen = Escape_Class->OpenEscapeZone;
		}
	}
}
