// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BurgerComponent.generated.h"

UENUM(BlueprintType)
enum class ESideDoneness : uint8
{
	Raw,
	Cooking,
	Cooked,
	Burned
};

USTRUCT(BlueprintType)
struct FBurgerSide
{
	GENERATED_BODY()

	UPROPERTY()
	float CookTime = 0.f;

	UPROPERTY()
	ESideDoneness Doneness = ESideDoneness::Raw;

	UPROPERTY()
	bool bIsCooking = false;
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DV1675_PROJECT_API UBurgerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	void UpdateMaterial();

	UPROPERTY()
	FBurgerSide SideA;

	UPROPERTY()
	FBurgerSide SideB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	float TimeToCook = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	float TimeToBurn = 30.f;

	UPROPERTY()
	bool bIsSideAUp = true;
	bool bIsOnGrill = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BurgerMesh;

	UPROPERTY(EditAnywhere, Category = "Cooking")
	UMaterialInterface* RawMaterial;

	UPROPERTY(EditAnywhere, Category = "Cooking")
	UMaterialInterface* CookedMaterial;

	UPROPERTY(EditAnywhere, Category = "Cooking")
	UMaterialInterface* BurnedMaterial;


public:	
	// Sets default values for this component's properties
	UBurgerComponent();

	UFUNCTION(BlueprintCallable)
	void StartCooking()
	{
		bIsOnGrill = true;
		SetComponentTickEnabled(true);
	}

	UFUNCTION(BlueprintCallable)
	void StopCooking()
	{
		bIsOnGrill = false;
		SetComponentTickEnabled(false);
	}

	UFUNCTION(BlueprintCallable)
	void FlipBurger()
	{
		bIsSideAUp = !bIsSideAUp;

		// If we're currently cooking, switch which side is active
		FBurgerSide& NewActiveSide = bIsSideAUp ? SideB : SideA;
		FBurgerSide& OldActiveSide = bIsSideAUp ? SideA : SideB;

		if (OldActiveSide.bIsCooking)
		{
			OldActiveSide.bIsCooking = false;
			NewActiveSide.bIsCooking = true;
		}

		UpdateMaterial();
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
