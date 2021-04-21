#include "DBDProject.h"
#include "EscapeZone.h"
#include "CCharacter.h"

AEscapeZone::AEscapeZone()
{
	PrimaryActorTick.bCanEverTick = true;
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	EscapeZoneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ESCAPEZONEMESH"));

	RootComponent = Trigger;
	EscapeZoneMesh->SetupAttachment(RootComponent);
	

	Trigger->SetCollisionProfileName(TEXT("Escape Zone"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ESCAPEMESH(TEXT(
		"/Game/DBD/Mesh/EscapeZone/Animation/NewFolder/Common_Exit_01_Ani.Common_Exit_01_Ani"));

	if (ESCAPEMESH.Succeeded())
	{
		EscapeZoneMesh->SetSkeletalMesh(ESCAPEMESH.Object);
	}

	EscapeZoneMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> EscapeZone_ANIM(TEXT
	("/Game/DBD/Mesh/EscapeZone/Animation/NewFolder/Common_Exit_01_Skeleton_AnimBlueprint.Common_Exit_01_Skeleton_AnimBlueprint_C"));

	if (EscapeZone_ANIM.Succeeded())
	{
		EscapeZoneMesh->SetAnimInstanceClass(EscapeZone_ANIM.Class);
	}

	OpenEscapeZone = false;
}

void AEscapeZone::BeginPlay()
{
	Super::BeginPlay();
}

void AEscapeZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEscapeZone::PostInitializeComponents()
{
	Super::PostInitializeComponents();	
	AnimInstance = EscapeZoneMesh->GetAnimInstance();
}



