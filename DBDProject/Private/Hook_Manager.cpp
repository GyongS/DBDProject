#include "DBDProject.h"
#include "Hook_Manager.h"

AHook_Manager::AHook_Manager()
{
	PrimaryActorTick.bCanEverTick = true;
	PersonHangCheckCol = CreateDefaultSubobject<USphereComponent>(TEXT("PERSONHITCHECKCOL"));
	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HOOKMESH"));

	PersonHangCheckCol->SetupAttachment(HookMesh);

	PersonHangCheckCol->SetSphereRadius(15.f);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Hook(TEXT("/Game/DBD/Mesh/Background/Common/Common_Hook_01"));

	if (Hook.Succeeded())
	{
		HookMesh->SetStaticMesh(Hook.Object);
	}
}

void AHook_Manager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHook_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

