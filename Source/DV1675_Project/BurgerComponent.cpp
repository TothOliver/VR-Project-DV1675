// Fill out your copyright notice in the Description page of Project Settings.


#include "BurgerComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UBurgerComponent::UBurgerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	// ...
}

<<<<<<< HEAD
//a
=======

>>>>>>> parent of 6f6f9dc (Revert "Merge branch 'burger'")
// Called when the game starts
void UBurgerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Automatically find mesh on owner
	BurgerMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();

	if (!BurgerMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("BurgerMesh not found on %s"), *GetOwner()->GetName());
		return;
	}

	// Set initial materials (both raw)
	UpdateMaterial();
}

// Called every frame
void UBurgerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FBurgerSide& ActiveSide = bIsSideAUp ? SideB : SideA;

	if (!ActiveSide.bIsCooking || ActiveSide.Doneness == ESideDoneness::Burned)
		return;

	ActiveSide.CookTime += DeltaTime;

	if (ActiveSide.CookTime >= TimeToBurn)
	{
		ActiveSide.Doneness = ESideDoneness::Burned;
	}
	else if (ActiveSide.CookTime >= TimeToCook)
	{
		ActiveSide.Doneness = ESideDoneness::Cooked;
	}
	else
	{
		ActiveSide.Doneness = ESideDoneness::Cooking;
	}

	UpdateMaterial();
}

void UBurgerComponent::UpdateMaterial()
{
	if (!BurgerMesh) return;

	// Update BOTH sides independently
	auto UpdateSideMaterial = [&](const FBurgerSide& Side, int32 MaterialIndex)
		{
			UMaterialInterface* MatToApply = nullptr;

			switch (Side.Doneness)
			{
			case ESideDoneness::Raw:
				MatToApply = RawMaterial;
				break;

			case ESideDoneness::Cooking:
			case ESideDoneness::Cooked:
				MatToApply = CookedMaterial;
				break;

			case ESideDoneness::Burned:
				MatToApply = BurnedMaterial;
				break;
			}

			if (MatToApply)
			{
				BurgerMesh->SetMaterial(MaterialIndex, MatToApply);
			}
		};

	// Element 0 = Side A
	// Element 1 = Side B
	UpdateSideMaterial(SideA, 0);
	UpdateSideMaterial(SideB, 1);
}
