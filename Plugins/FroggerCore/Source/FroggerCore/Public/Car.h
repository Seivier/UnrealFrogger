// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Car.generated.h"

UCLASS()
class FROGGERCORE_API ACar : public AActor
{
	GENERATED_BODY()
public:
    // SkeleticalMesh for the car
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Car")
    class USkeletalMeshComponent* Mesh;
	
	// Box Collision for the car
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Car")
	class UBoxComponent* Collision;

	// Speed of the car
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSpeed;
	
	// Sounds for the car
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class UAudioComponent* DrivingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* CrashSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
	class ACarSpawner* Spawner;
	
private:
	// Root
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Car", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	bool bEnabled;
	
public:	
	// Sets default values for this actor's properties
	ACar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Move(float DeltaTime);

	void Toggle();
	bool IsEnabled() const;

	// Used for setting variables, its called when a Spawner spawns the car
	UFUNCTION(BlueprintImplementableEvent, Category = "Car")
	void Spawn();

	
};
