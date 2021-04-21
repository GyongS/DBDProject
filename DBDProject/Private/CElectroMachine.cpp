#include "DBDProject.h"
#include "CElectroMachine.h"

ACElectroMachine::ACElectroMachine()
{
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	GeneratorSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GeneratorSkel"));

	/*Col1 = CreateDefaultSubobject<USphereComponent>(TEXT("CHECKCOL1"));
	Col2 = CreateDefaultSubobject<USphereComponent>(TEXT("CHECKCOL2"));
	Col3 = CreateDefaultSubobject<USphereComponent>(TEXT("CHECKCOL3"));
	Col4 = CreateDefaultSubobject<USphereComponent>(TEXT("CHECKCOL4"));*/

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	GeneratorSkelMesh->SetupAttachment(RootComponent);
	/*Col1->SetupAttachment(RootComponent);
	Col2->SetupAttachment(RootComponent);
	Col3->SetupAttachment(RootComponent);
	Col4->SetupAttachment(RootComponent);*/


	static ConstructorHelpers::FObjectFinder<UStaticMesh> Machine(TEXT("/Game/DBD/Mesh/Background/Common/Common_Generator_01.Common_Generator_01"));

	if (Machine.Succeeded())
	{
		Box->SetStaticMesh(Machine.Object);
	}

	Box->SetRelativeLocation(FVector(0.f, -3.5f, -30.f));

	Trigger->SetCollisionProfileName(TEXT("Electro Machine"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));
}

void ACElectroMachine::BeginPlay()
{
	Super::BeginPlay();
}

void ACElectroMachine::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACElectroMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}