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

		UFUNCTION(BlueprintCallable, Category = "GameGeneratedActor")
		void SetMaterial(UMaterialInterface* Material);

	UPROPERTY(EditAnywhere)
		float YZ_angle;
	UPROPERTY(EditAnywhere)
		float XZ_angle;
	UPROPERTY(EditAnywhere)
		float XY_angle;
	UPROPERTY(EditAnywhere)
		float XW_angle;
	UPROPERTY(EditAnywhere)
		float YW_angle;
	UPROPERTY(EditAnywhere)
		float ZW_angle;

	UPROPERTY(EditAnywhere)
		bool use_YZ;
	UPROPERTY(EditAnywhere)
		bool use_XZ;
	UPROPERTY(EditAnywhere)
		bool use_XY;
	UPROPERTY(EditAnywhere)
		bool use_XW = true;
	UPROPERTY(EditAnywhere)
		bool use_YW = true;
	UPROPERTY(EditAnywhere)
		bool use_ZW = true;

	UPROPERTY(EditAnywhere)
		float theta1 = 1;
	UPROPERTY(EditAnywhere)
		float theta2 = 1;
	UPROPERTY(EditAnywhere)
		float theta3 = 1;

private:
	float RunningTime;
	TArray<FCustomMeshTriangle> GetTriangles(float time);
	FVector4 Rotate(FVector4 v, FVector4* m);
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