#include "DBDProject.h"
#include "Killer_AnimInstance.h"
#include "C_Killer.h"
#include "CCharacter.h"

UKiller_AnimInstance::UKiller_AnimInstance()
{
	CurrentPawnSpeed = 0.f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT(
		"/Game/DBD/Mesh/Killer/Animation/Killer_Attack_Montage.Killer_Attack_Montage"));

	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UKiller_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		KillerClass = Cast<AC_Killer>(Pawn);			
		PersonClass = Cast<ACCharacter>(Pawn);
		isGrab = KillerClass->isGrabPerson;
		isHang = KillerClass->isHangPerson;
	}

}

void UKiller_AnimInstance::PlayAttackMontage()
{
	if (KillerClass)
	{
		Montage_Play(AttackMontage, 1.f);
	}
}

void UKiller_AnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UKiller_AnimInstance::AnimNotify_isGrabPerson()
{
	OnGrabisPersonCheck.Broadcast();
}

void UKiller_AnimInstance::AnimNotify_PickUpCheck()
{
	OnPickUpCheck.Broadcast();
}

void UKiller_AnimInstance::AnimNotify_HangingCheck()
{
	OnHangingCheck.Broadcast();
}

void UKiller_AnimInstance::AnimNotify_HangingEnd()
{
	OnHangingEnd.Broadcast();
}

void UKiller_AnimInstance::AnimNotify_HangingStart()
{
	OnHangingStart.Broadcast();
}
