#include "DBDProject.h"
#include "C_Killer.h"
#include "CCharacter.h"
#include "Hook_Manager.h"
#include "Killer_AnimInstance.h"
#include "..\Public\C_Killer.h"
#include "DrawDebugHelpers.h"

#pragma region Init
AC_Killer::AC_Killer()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	InteractionCol = CreateDefaultSubobject<USphereComponent>(TEXT("INTERACTIONCOL"));
	ShoulderCol = CreateDefaultSubobject<UBoxComponent>(TEXT("SHOULDERCOL"));
	
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	InteractionCol->SetupAttachment(GetMesh());
	InteractionCol->SetSphereRadius(100.f);
	InteractionCol->SetRelativeLocation(FVector(0.f, 0.f, 90.f));

	ShoulderCol->SetupAttachment(GetMesh());
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	//SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	InteractionCol->OnComponentBeginOverlap.AddDynamic(this, &AC_Killer::OnOverlapBegin);
	InteractionCol->OnComponentEndOverlap.AddDynamic(this, &AC_Killer::OnOverlapEnd);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Killer(TEXT(
		"/Game/DBD/Mesh/Killer/Animation/Rigging.Rigging"));

	if (Killer.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(Killer.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> Killer_ANIM(TEXT(
		"/Game/DBD/Mesh/Killer/Animation/Killer_AnimBlueprint.Killer_AnimBlueprint_C"));

	if (Killer_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Killer_ANIM.Class);
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Killer"));

	SpringArm->TargetArmLength = -15.f;
	//SpringArm->SocketOffset = (FVector(-10.f, 0.f, 40.f));
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->MaxWalkSpeed = 150.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	isAttack = false;
	EndAttackState();	
	
	AttackRange = 200.f;
	AttackRadius = 30.f;
}
#pragma endregion


void AC_Killer::BeginPlay()
{
	Super::BeginPlay();
}

void AC_Killer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC_Killer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	KillerAnim = Cast<UKiller_AnimInstance>(GetMesh()->GetAnimInstance());

	KillerAnim->OnMontageEnded.AddDynamic(this, &AC_Killer::OnAttackMontageEnded);

	KillerAnim->OnAttackHitCheck.AddUObject(this , &AC_Killer::DamageCheck);
	KillerAnim->OnGrabisPersonCheck.AddUObject(this, &AC_Killer::GrabCheck);
	KillerAnim->OnPickUpCheck.AddUObject(this, &AC_Killer::PickedUpPerson);
	KillerAnim->OnHangingCheck.AddUObject(this, &AC_Killer::HangingPerson);
	KillerAnim->OnHangingStart.AddUObject(this, &AC_Killer::HangingStart);
	KillerAnim->OnHangingEnd.AddUObject(this, &AC_Killer::HangingEnd);
}

#pragma region Trigger Enter
void AC_Killer::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(ACCharacter::StaticClass()))
	{
		PersonClass = Cast<ACCharacter>(OtherActor);
		if (PersonClass->LifeCount <= 1 && PersonClass->isGrab)
		{
			//ABLOGSCREEN("Grab is Person");
		}
	}

	if (OtherActor->IsA(AHook_Manager::StaticClass()))
	{
		HookClass = Cast<AHook_Manager>(OtherActor);
		if (PersonClass->LifeCount <= 1 && PersonClass->isGrab)
		{
			isHookOverlapCheck = true;
			//ABLOGSCREEN("Hang is Person");
		}
	}
}

void AC_Killer::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ACCharacter::StaticClass()))
	{
	}
	if (OtherActor->IsA(AHook_Manager::StaticClass()))
	{
		isHookOverlapCheck = false;
		isHangPerson = false;
	}
}

#pragma endregion

#pragma region Input
void AC_Killer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 이동 회전
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AC_Killer::Horizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AC_Killer::Vertical);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AC_Killer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AC_Killer::Turn);
	PlayerInputComponent->BindAction(TEXT("KillerInteraction"), IE_Pressed ,this, &AC_Killer::Interaction);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AC_Killer::Attack);
}

void AC_Killer::Horizontal(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(
		GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void AC_Killer::Vertical(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(
		GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void AC_Killer::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AC_Killer::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void AC_Killer::Interaction()
{

	if (!isGrabPerson && PersonClass->isGrab && !isHangPerson && !isHookOverlapCheck)
	{
		isGrabPerson = true;	
		//ABLOGSCREEN("Picked Up!!");
	}		
	if (isHookOverlapCheck && !isHangPerson)
	{
		isHangPerson = true;
		//ABLOGSCREEN("Hanging!!");
	}

}
#pragma endregion

#pragma region Attack
void AC_Killer::Attack()
{
	if (isAttack)
	{
		if (CanNextAttack) return;		
	}
	else
	{
		StartAttackState();
		isAttack = true;
	}

	if (nullptr == KillerAnim) return;
	KillerAnim->PlayAttackMontage();	
}

void AC_Killer::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	isAttack = false;
	EndAttackState();
}
void AC_Killer::StartAttackState()
{
	CanNextAttack = true;
}
void AC_Killer::EndAttackState()
{
	CanNextAttack = false;
}
void AC_Killer::DamageCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(50.f),
		Params);

	PersonClass = Cast<ACCharacter>(HitResult.Actor);
	// 공격 충돌체 생성
//#if ENABLE_DRAW_DEBUG
//
//	FVector TraceVec = GetActorForwardVector() * AttackRange;
//	FVector Center = GetActorLocation() + TraceVec * 0.5f;
//	float HalfHeight = AttackRange * 0.5f + AttackRadius;
//	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
//	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//	float DebugLifeTime = 5.f;
//
//	DrawDebugCapsule(GetWorld(),
//		Center,
//		HalfHeight,
//		AttackRadius,
//		CapsuleRot,
//		DrawColor,
//		false,
//		DebugLifeTime);
//#endif

	// 데미지 체크
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			//ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());						
			FPointDamageEvent DamageEvent;
			DamageEvent.HitInfo = HitResult;
			PersonClass->TakeDamage(1.f, DamageEvent, GetController(), this);
			PersonLifeCount -= 1.f;
			if (PersonLifeCount <= 0.f)
			{
				PersonLifeCount = 0.f;
			}
		}
	
	}
}
void AC_Killer::GrabCheck()
{
	//331.f, 0.f, 320.f		
	/*PersonClass->GetCapsuleComponent()->SetWorldLocationAndRotation(
		FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z), FRotator(-29.f, 0.f, -40.f));*/
	//PersonClass->GetCapsuleComponent()->SetRelativeRotation(FRotator(-29.f, 0.f, -40.f));
	isGrabPerson = false;
	SpringArm->TargetArmLength = -50;
	//ABLOGSCREEN("Grab");
}

void AC_Killer::PickedUpPerson()
{
	FName SocketName;
	SocketName = "GrabSocket";
	PersonClass->GetCapsuleComponent()->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld,
			true),
		SocketName);

	PersonClass->SetActorEnableCollision(ECollisionEnabled::NoCollision);


	//ABLOGSCREEN("Picked Up");
}
void AC_Killer::HangingPerson()
{
	PersonClass->isHang = true;
	isPersonHanging = true;
	PersonClass->GetCapsuleComponent()->AttachToComponent(
		HookClass->GetRootComponent(),
		FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld,
			true)
	);
		
	PersonClass->GetCapsuleComponent()->SetWorldLocationAndRotation(
		FVector(-650.0, 1520.0, 120.0), FRotator(0.f, 0.f, 0.f));

	//ABLOGSCREEN("Hanging");
}
void AC_Killer::HangingStart()
{
	SpringArm->TargetArmLength = 300;
}
void AC_Killer::HangingEnd()
{
	isHangPerson = false;		
	isGrabPerson = false;
}
#pragma endregion

