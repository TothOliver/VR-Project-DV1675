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


// Called when the game starts
void UBurgerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!BurgerMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("BurgerComponent: BurgerMesh not assigned on %s"), *GetOwner()->GetName());
		return;
	}

	DynamicMaterial = BurgerMesh->CreateAndSetMaterialInstanceDynamic(0);
	
}


// Called every frame
void UBurgerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FBurgerSide& ActiveSide = bIsSideAUp ? SideA : SideB;

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
	if (!DynamicMaterial) return;

	FBurgerSide& ActiveSide = bIsSideAUp ? SideA : SideB;

	float CookAlpha = FMath::Clamp(ActiveSide.CookTime / TimeToBurn, 0.f, 1.f);

	DynamicMaterial->SetScalarParameterValue("CookAmount", CookAlpha);
}
