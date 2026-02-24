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
	float TimeToCook = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	float TimeToBurn = 10.f;

	UPROPERTY()
	bool bIsSideAUp = true;

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
		SetComponentTickEnabled(true);

		FVector MeshUp = BurgerMesh->GetUpVector();

		// If the mesh's up vector points down in world space, it's flipped
		bool bNewSideAUp = MeshUp.Z > 0.f;

		if (bNewSideAUp != bIsSideAUp)
		{
			bIsSideAUp = bNewSideAUp;
		}

		FBurgerSide& ActiveSide = bIsSideAUp ? SideB : SideA;
		ActiveSide.bIsCooking = true;
	}

	UFUNCTION(BlueprintCallable)
	void StopCooking()
	{
		SetComponentTickEnabled(false);
		FBurgerSide& ActiveSide = bIsSideAUp ? SideB : SideA;
		ActiveSide.bIsCooking = false;
	}


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
