#include "DBDProject.h"
#include "CAnimInstance.h"
#include "CCharacter.h"
#include "EscapeZone.h"

UCAnimInstance::UCAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	CurrentMoveSpeed = 500.f;
	IsRunning = false;
	IsCrouch = false;
	IsRepair = false;
	IsInjured = false;
	IsProne = false;
	IsHanging = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PARKOUR_MONTAGE(TEXT(
		"/Game/DBD/Mesh/Character/Animation/Parkour_Montage.Parkour_Montage"));

	if (PARKOUR_MONTAGE.Succeeded())
	{
		ParkourMonstage = PARKOUR_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ESCAPE_MONTAGE(TEXT(
		"/Game/DBD/Mesh/Character/Animation/Person_Escape_Montage.Person_Escape_Montage"));

	if (ESCAPE_MONTAGE.Succeeded())
	{
		EscapeMontage = ESCAPE_MONTAGE.Object;
	}

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();

	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		PlayerClass = Cast<ACCharacter>(Pawn);
		if (Character)
		{
			//ABLOGSCREEN("Person");
			CurrentMoveSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
			IsCrouch = Character->GetCharacterMovement()->bWantsToCrouch;
			IsRepair = PlayerClass->b_Repair;
			IsHanging = PlayerClass->isHang;
			if (CurrentMoveSpeed > 500.f && CurrentPawnSpeed > 0)
			{
				IsRunning = true;
			}
			else
			{
				IsRunning = false;
			}
			if (PlayerClass->LifeCount <= 1)
			{
				IsInjured = false;
				IsProne = true;
			}
			else if (PlayerClass->LifeCount <= 2)
			{
				IsInjured = true;
			}
		}

	}
}

void UCAnimInstance::PlayParkourMontage()
{
	Montage_Play(ParkourMonstage, 1.f);
}

void UCAnimInstance::PlayEscapeMontage()
{
	Montage_Play(EscapeMontage, 0.8f);
}
