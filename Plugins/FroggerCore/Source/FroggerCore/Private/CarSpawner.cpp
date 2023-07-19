// Fill out your copyright notice in the Description page of Project Settings.


#include "CarSpawner.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ACarSpawner::ACarSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	// Colliders
	InitCheck = CreateDefaultSubobject<USphereComponent>(TEXT("InitCheck"));
	InitCheck->SetupAttachment(Root);
	InitCheck->SetSphereRadius(1.f);
	InitCheck->SetRelativeLocation(FVector(100.f, 0.0f, 0.f));
	InitCheck->OnComponentBeginOverlap.AddDynamic(this, &ACarSpawner::DisableSpawn);
	InitCheck->OnComponentEndOverlap.AddDynamic(this, &ACarSpawner::EnableSpawn);

	EndCheck = CreateDefaultSubobject<USphereComponent>(TEXT("EndCheck"));
	EndCheck->SetupAttachment(Root);
	EndCheck->SetSphereRadius(1.f);
	EndCheck->SetRelativeLocation(FVector(-100.f, 0.0f, 0.f));
	EndCheck->OnComponentEndOverlap.AddDynamic(this, &ACarSpawner::CheckOverflow);

	// Parameters
	Chance = 5.f;
	InitialSpeed = 100.f;
	StorageLocation = FVector(0.f, 0.f, 0.f);
	Angle = 0.f;

	bEnabled = true;
	CurrentChance = 0.f;

	// Array
	Cars = TArray<ACar*>();
	
}

// Called when the game starts or when spawned
void ACarSpawner::BeginPlay()
{
	Super::BeginPlay();

	auto start = InitCheck->GetComponentLocation();
	auto end = EndCheck->GetComponentLocation();
	auto direction = end - start;
	// UE_LOG(LogTemp, Warning, TEXT("Angle: %f, Direction: %f %f %f"), Angle, direction.X, direction.Y, direction.Z);
	auto distance = direction.Size();
	direction.Normalize();

	Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(direction, FVector::ForwardVector)));
	if (FVector::DotProduct(direction, FVector::RightVector) < 0.f)
		Angle *= -1.f;
	// Print angle
	// UE_LOG(LogTemp, Warning, TEXT("Angle: %f, Direction: %f %f %f"), Angle, direction.X, direction.Y, direction.Z);

	

	// Spawn first car

	AttemptSpawn(Chance);
	auto First = Cars[0];
	
	// auto first = GetWorld()->SpawnActor<ACar>(CarClass, start, FRotator(0.f, Angle, 0.f));
	// first->Spawner = this;
	// first->Speed = InitialSpeed;
	//
	// // Create storage
	// Cars.Add(first);

	// Set locations
	auto distanceOfFirstCar = First->GetActorLocation() - start;
	while (distanceOfFirstCar.Size() < distance)
	{
		// Simulate movement at 15 fps
		for (auto Car : Cars)
		{
			Car->Move(1.f / 8.f);
		}

				
		// Add a car
		AttemptSpawn(1.f / 8.f);
		// auto Temp = GetWorld()->SpawnActor<ACar>(CarClass, start, FRotator(0.f, Angle, 0.f));
		// Temp->Speed = InitialSpeed;
		// Temp->Spawn();
		// Cars.Add(Temp);
		
		// Update distance
		distanceOfFirstCar = First->GetActorLocation() - start;
	}
	First->Toggle(); // Disable
	First->SetActorLocation(StorageLocation);
	
	
}

void ACarSpawner::EnableSpawn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ACar>(OtherActor) && OtherComp)
	{
		if (Cast<UBoxComponent>(OtherComp))
			bEnabled = true;
	}
}

void ACarSpawner::DisableSpawn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACar>(OtherActor) && OtherComp)
	{
		if (Cast<UBoxComponent>(OtherComp))
			bEnabled = false;
	}
}

void ACarSpawner::CheckOverflow(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACar* Car = Cast<ACar>(OtherActor);
	if (Car && OtherComp)
	{
		if (Cast<UBoxComponent>(OtherComp))
		{
			DisableCar(Car);		
		}		
	}
}

// Called every frame
void ACarSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!bEnabled) return;
	
	AttemptSpawn(DeltaTime);
}

void ACarSpawner::AttemptSpawn(float DeltaTime)
{
	DeltaTime *= FMath::RandRange(1.f, 2.f);
	CurrentChance += DeltaTime;
	if (CurrentChance < Chance) return;
	
	// Get first car inactive
	auto Inactive = GetFirstAvailableCar();
	if (!Inactive)
	{
		// Spawn a new car
		Inactive = GetWorld()->SpawnActor<ACar>(CarClass, StorageLocation, FRotator::ZeroRotator);
		Inactive->Spawner = this;
		Inactive->Toggle(); // Disable
		Cars.Add(Inactive);
	}

	// not enabled
	Inactive->Toggle(); // Enable
	Inactive->SetActorLocation(InitCheck->GetComponentLocation());
	Inactive->Speed = InitialSpeed;
	Inactive->SetActorRotation(FRotator(0.f, Angle, 0.f));
	Inactive->Spawn();

	CurrentChance = 0.f;
}

void ACarSpawner::DisableCar(ACar* Car)
{
	// Check if car is in array
	if (Cars.Contains(Car))
	{
		Car->Toggle();
		Car->SetActorLocation(StorageLocation);
	}
}

ACar* ACarSpawner::GetFirstAvailableCar()
{
	for (auto car : Cars)
	{
		if (!car->IsEnabled())
			return car;
	}
	return nullptr;
}

