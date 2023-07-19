// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Car.h"
#include "CarSpawner.generated.h"


UCLASS()
class FROGGERCORE_API ACarSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Colliders
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collider")
	class USphereComponent* InitCheck;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collider")
	USphereComponent* EndCheck;

	// Params
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float Chance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float InitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	FVector StorageLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	UClass* CarClass;
	
private:
	// Root
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CarSpawner", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	bool bEnabled;
	float CurrentChance;
	float Angle;

	// Cars Available
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CarSpawner", meta = (AllowPrivateAccess = "true"))
	TArray<ACar*> Cars;
	
public:	
	// Sets default values for this actor's properties
	ACarSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void EnableSpawn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex);

	UFUNCTION()
	void DisableSpawn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CheckOverflow(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   int32 OtherBodyIndex);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AttemptSpawn(float DeltaTime);

	// Blueprint callable functions
	UFUNCTION(BlueprintCallable, Category = "CarSpawner")
	void DisableCar(ACar* Car);

private:
	ACar* GetFirstAvailableCar();

};
