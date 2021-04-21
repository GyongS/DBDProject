#include "DBDProject.h"
#include "PalletActor.h"

APalletActor::APalletActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	/*
	PalletCol1 = CreateDefaultSubobject<USphereComponent>(TEXT("PALLETCOL0"));
	PalletCol2 = CreateDefaultSubobject<USphereComponent>(TEXT("PALLETCOL1"));*/


	RootComponent = Trigger;
	Box->SetupAttachment(Trigger);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Pallet(TEXT("/Game/DBD/Mesh/Background/Common/Common_Pallets_01.Common_Pallets_01"));
	if (Pallet.Succeeded())
	{
		Box->SetStaticMesh(Pallet.Object);
	}

	Box->SetRelativeLocation(FVector(0.f, -3.5f, -30.f));

	Trigger->SetCollisionProfileName(TEXT("PalletActor"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));
}

void APalletActor::BeginPlay()
{
	Super::BeginPlay();
}

void APalletActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APalletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}