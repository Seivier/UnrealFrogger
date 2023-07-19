// Fill out your copyright notice in the Description page of Project Settings.


#include "Car.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ACar::ACar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	// Car
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disable collision

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	Collision->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	// Movement
	Speed = 100.0f;
	Acceleration = 0.0f;
	MaxSpeed = 100.0f;
	MinSpeed = 100.0f;

	// Sounds
	DrivingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DrivingSound"));
	DrivingSound->SetupAttachment(RootComponent);
	
	CrashSound = CreateDefaultSubobject<USoundBase>(TEXT("CrashSound"));

	bEnabled = true;
}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACar::Move(float DeltaTime)
{
	auto Forward = GetActorForwardVector();
	auto Location = GetActorLocation();
	Speed += Acceleration * DeltaTime;
	Speed = FMath::Clamp(Speed, MinSpeed, MaxSpeed);
	Location += Forward * Speed * DeltaTime;
	SetActorLocation(Location);
}

void ACar::Toggle()
{
	bEnabled = !bEnabled;
}

bool ACar::IsEnabled() const
{
	return bEnabled;
}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bEnabled)
		return;
	
	Move(DeltaTime);

	// Play engine sound at location
	// UGameplayStatics::PlaySoundAtLocation(this, DrivingSound, GetActorLocation());

}
