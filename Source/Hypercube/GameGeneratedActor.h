// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "CustomMeshComponent.h"
#include "GameGeneratedActor.generated.h"

/**
*
*/
UCLASS()
class AGameGeneratedActor : public AActor
{
	GENERATED_UCLASS_BODY()

private:
	float RunningTime;
	TArray<FCustomMeshTriangle> GetTriangles(float time);
	float DotProduct(FVector4 rhs, FVector4 lhs);
	FVector4 Normalize(FVector4 v);
	TArray<FVector> GetProjector(FVector4 planeNormal);
	FVector Project(FVector4 a, TArray<FVector> proj);
	void GenerateTriangles(FVector a, FVector b, FVector c, FVector d, TArray<FCustomMeshTriangle>& triangles);
	void GenerateTriangles(FVector a, FVector b, FVector c, TArray<FCustomMeshTriangle>& triangles);
	void InvertTriangle(FCustomMeshTriangle& triangle);


public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

};