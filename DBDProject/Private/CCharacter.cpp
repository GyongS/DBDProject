#include "DBDProject.h"
#include "CCharacter.h"
#include "CElectroMachine.h"
#include "PalletActor.h"
#include "EscapeZone.h"
#include "C_Killer.h"
#include "CAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

#pragma region Init
ACCharacter::ACCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	ServeCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SERVECAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	ServeCamera->SetupAttachment(SpringArm);

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("INTERACTIONSPHERE"));

	InteractionSphere->SetupAttachment(GetMesh());
	InteractionSphere->SetSphereRadius(100.f);
	InteractionSphere->SetRelativeLocation(FVector(0.f, 0.f, 90.f));

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACCharacter::OnOverlapBegin);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ACCharacter::OnOverlapEnd);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));	

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Person(TEXT(
		"/Game/DBD/Mesh/Character/Animation/PersonSkelMesh.PersonSkelMesh"));

	if (Person.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(Person.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> Person_ANIM(TEXT(
		"/Game/DBD/Mesh/Character/Animation/Person_Skeleton_AnimBlueprint.Person_Skeleton_AnimBlueprint_C"));

	if (Person_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Person_ANIM.Class);
	}

	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 150.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ACCharacter"));	
}
#pragma endregion

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	if (MinMachineGauge >= MaxMachineGauge)
	{
		b_Repair = false;
		b_MachineOverlapCheck = false;
	}
}

void ACCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG(Warning, TEXT("Post Init"));

	PersonAnimInstance = Cast<UCAnimInstance>(GetMesh()->GetAnimInstance());
	
	PersonAnimInstance->OnMontageEnded.AddDynamic(this, &ACCharacter::OnPersonMontageEnded);

}

// Killer 한테 맞았을 때
float ACCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	LifeCount -= FinalDamage;

	/*ABLOGSCREEN("Current LifeCount : %f", LifeCount);
	ABLOGSCREEN("Actor : %s Hit Damage : %f", *GetName(), FinalDamage);*/

	isDead = LifeCount <= 0.f ? true : false;	
	isGrab = LifeCount <= 1.f ? true : false;		

	if (isDead)
	{
		//ABLOGSCREEN("is DEAD");
		LifeCount = 0.f;
	}

	return FinalDamage;
}

#pragma region Input Component
void ACCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 이동 회전
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ACCharacter::Horizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ACCharacter::Vertical);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACCharacter::Turn);

	// 걷기 뛰기
	PlayerInputComponent->BindAction(TEXT("LeftShift"), IE_Pressed, this, &ACCharacter::LeftShift);
	PlayerInputComponent->BindAction(TEXT("LeftShift"), IE_Released, this, &ACCharacter::LeftShift);

	// 앉기 바인딩
	PlayerInputComponent->BindAction(TEXT("LeftCtrl"), EInputEvent::IE_Pressed, this, &ACCharacter::Crouching);
	PlayerInputComponent->BindAction(TEXT("LeftCtrl"), EInputEvent::IE_Released, this, &ACCharacter::Crouching);

	// 발전기 수리
	PlayerInputComponent->BindAction(TEXT("Repair"), EInputEvent::IE_Pressed, this, &ACCharacter::Interaction);
	PlayerInputComponent->BindAction(TEXT("Repair"), EInputEvent::IE_Released, this, &ACCharacter::Interaction);

	// 판자 넘기
	PlayerInputComponent->BindAction(TEXT("Parkour"), EInputEvent::IE_Pressed, this, &ACCharacter::ParKour);
}
#pragma endregion

#pragma region OnTriggerEnter & Exit
//오브젝트가 플레이어와 충돌 했을 때 호출되는 이벤트
void ACCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (MinMachineGauge <= MaxMachineGauge)
	{
		if (OtherActor->IsA(ACElectroMachine::StaticClass()) && LifeCount > 2)
		{
			
			ElectroMachine = Cast<ACElectroMachine>(OtherActor);
			b_MachineOverlapCheck = true;
			//ABLOGSCREEN("%s", *(ElectroMachine->GetName()));

		}		
	}	

	if (OtherActor->IsA(APalletActor::StaticClass()) && LifeCount > 2)
	{
		PalletClass = Cast<APalletActor>(OtherActor);
		b_CheckPallet = true;
		//ABLOGSCREEN("%s", *(PalletClass->GetName()));
	}

	if (OtherActor->IsA(AEscapeZone::StaticClass()))
	{
		EscapeZoneClass = Cast<AEscapeZone>(OtherActor);
		if (MinMachineGauge >= MaxMachineGauge)
		{
			b_EscapeZone = true;
		}
		//ABLOGSCREEN("%s", *(EscapeZoneClass->GetName()));
	}
	
}

//오브젝트가 플레이어와 충돌에서 빠져나갔을 때 호출되는 이벤트
void ACCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MinMachineGauge <= MaxMachineGauge)
	{
		if (OtherActor->IsA(ACElectroMachine::StaticClass()))
		{
			b_MachineOverlapCheck = false;
			ElectroMachine = nullptr;
		}	
	}
	if (OtherActor->IsA(APalletActor::StaticClass()))
	{
		b_CheckPallet = false;
	}

	if (OtherActor->IsA(AEscapeZone::StaticClass()))
	{
		b_EscapeZone = false;
	}
}
#pragma endregion



#pragma region Input

// 좌우 이동
void ACCharacter::Horizontal(float NewAxisValue)
{
	if (!b_Repair)
	{
		AddMovementInput(FRotationMatrix(
			GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
	}
}

void ACCharacter::Vertical(float NewAxisValue)
{
	if (!b_Repair)
	{
		AddMovementInput(FRotationMatrix(
			GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
	}
}

// 회전
void ACCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void ACCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

// 걷기 뛰기
void ACCharacter::LeftShift()
{
	if (!b_Running && LifeCount >= 2)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		b_Running = true;
		ABLOG_S(Warning);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 150.f;
		b_Running = false;
	}
}

// 앉은 상태
void ACCharacter::Crouching()
{
	if (!GetCharacterMovement()->bWantsToCrouch)
	{
		GetCharacterMovement()->bWantsToCrouch = true;
	}
	else
	{
		GetCharacterMovement()->bWantsToCrouch = false;
	}
}


// 상호 작용 버튼ww
void ACCharacter::Interaction()
{
	if (MinMachineGauge >= MaxMachineGauge)
	{
		b_Repair = false;
		b_MachineOverlapCheck = false;
		MinMachineGauge = 1000;		
	}
	else if (!b_Repair && b_MachineOverlapCheck && LifeCount > 2)
	{
		b_Repair = true;
		// if(범위 안에서 가장 가까운 자리로 이동하여 발전기 수리)		
		
		// Generator Look At
		FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ElectroMachine->GetActorLocation());
		GetCapsuleComponent()->SetRelativeRotation(FRotator(GetActorRotation().Pitch, PlayerRot.Yaw, PlayerRot.Roll));
		//GetCapsuleComponent()->SetRelativeLocation(FVector(GetActorLocation().X, ElectroMachine->GetActorLocation().Y, GetActorLocation().Z));

	}	
	else
	{
		b_Repair = false;		
	}

	if (b_EscapeZone)
	{
		// 비상 탈출구 위에서 뛰어 내리는 동작!
		GetCapsuleComponent()->SetRelativeLocation(FVector(GetActorLocation().X, EscapeZoneClass->GetActorLocation().Y, GetActorLocation().Z));
		PersonAnimInstance->PlayEscapeMontage();
	}
}

void ACCharacter::ParKour()
{	
	if (b_Parkour) return;

	if (b_CheckPallet)
	{	
		// 한 번에 넘어갈 수 있는 위치에서 애니메이션 시작
		FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PalletClass->GetActorLocation());
		GetCapsuleComponent()->SetRelativeRotation(FRotator(GetActorRotation().Pitch, PlayerRot.Yaw, PlayerRot.Roll));
		//GetCapsuleComponent()->SetRelativeLocation(FVector(PalletClass->GetActorLocation().X, PalletClass->GetActorLocation().Y,GetActorLocation().Z));
		PersonAnimInstance->PlayParkourMontage();
		b_Parkour = true;
	}
}

void ACCharacter::OnPersonMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	b_CheckPallet = true;
	//ABLOGSCREEN("%s", b_EscapeZone ? TEXT("true") : TEXT("False"));

	if (b_CheckPallet && !b_EscapeZone)
	{
		//ABLOGSCREEN("End ParkourMontage");		
		GetCapsuleComponent()->SetRelativeRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
		b_Parkour = false;
		b_CheckPallet = false;
	}

	if (b_EscapeZone)
	{
		//ABLOGSCREEN("End EscapeMontage");
		GetCapsuleComponent()->SetRelativeLocation(FVector(GetActorLocation().X, GetActorLocation().Y, -10.f));
	}
}
#pragma endregion