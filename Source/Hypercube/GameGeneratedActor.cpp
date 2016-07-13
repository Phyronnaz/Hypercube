// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Hypercube.h"
#include "GameGeneratedActor.h"

AGameGeneratedActor::AGameGeneratedActor(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;

	UCustomMeshComponent* mesh = PCIP.CreateDefaultSubobject<UCustomMeshComponent>(this, TEXT("GeneratedMesh"));

	mesh->SetCustomMeshTriangles(GetTriangles(RunningTime));

	RootComponent = mesh;
}

TArray<FCustomMeshTriangle> AGameGeneratedActor::GetTriangles(float time) {
	TArray<FCustomMeshTriangle> triangles;

	FVector4 planeNormal = FVector4(1, 1, 1, 1);

	TArray<FVector> projector = GetProjector(planeNormal);

	FCustomMeshTriangle tri;

	for (int i = 0; i < 4; i++) {
		for (int a = -1; a < 2; a += 2) {
			for (int j = 0; j < i; j++) {
				for (int b = -1; b < 2; b += 2) {
					TArray<FVector> vectors;

					for (int c = -1; c < 2; c += 2) {
						for (int d = -1; d < 2; d += 2) {
							int k = 0;
							while (i == k || j == k) {
								k++;
							}

							int l = 0;
							while (i == l || j == l || k == l) {
								l++;
							}

							FVector4 v;
							v[i] = a;
							v[j] = b;
							v[k] = c;
							v[l] = d;
							FVector projection = Project(v, projector);
							vectors.Add(projection);

						}
					}

					GenerateTriangles(vectors[0], vectors[1], vectors[2], vectors[3], triangles);
				}
			}
		}
	}

	/*for (int i = 0; i < 3; i++) {
		for (int j = -1; j < 2; j += 2) {
			FVector a;
			FVector b;
			FVector c;
			FVector d;

			a[i] = j;
			b[i] = j;
			c[i] = j;
			d[i] = j;

			a[(i + 1) % 3] = -1;
			a[(i + 2) % 3] = 1;

			b[(i + 1) % 3] = 1;
			b[(i + 2) % 3] = 1;

			c[(i + 1) % 3] = 1;
			c[(i + 2) % 3] = -1;

			d[(i + 1) % 3] = -1;
			d[(i + 2) % 3] = -1;

			GenerateTriangles(a, b, c, d, triangles);
		}
	}*/

	return triangles;
}

TArray<FVector> AGameGeneratedActor::GetProjector(FVector4 planeNormal) {
	FVector4 basis[] = { FVector4(planeNormal[0], 0.f, 0.f, -planeNormal[0] / planeNormal[3]) ,
						FVector4(0.f, planeNormal[1], 0.f, -planeNormal[1] / planeNormal[3]) ,
						FVector4(0.f, 0.f, planeNormal[2], -planeNormal[2] / planeNormal[3]) };
	basis[0] = Normalize(basis[0]);

	basis[1] += -basis[0] * DotProduct(basis[0], basis[1]);
	basis[1] = Normalize(basis[1]);

	basis[2] += -basis[0] * DotProduct(basis[0], basis[2]);
	basis[2] += -basis[1] * DotProduct(basis[1], basis[2]);
	basis[2]  = Normalize(basis[2]);
	FVector projector[] = { FVector(basis[0][0], basis[1][0], basis[2][0]),
							FVector(basis[0][1], basis[1][1], basis[2][1]),
							FVector(basis[0][2], basis[1][2], basis[2][2]),
							FVector(basis[0][3], basis[1][3], basis[2][3]) };
	TArray<FVector> tprojector;
	tprojector.Append(projector, ARRAYSIZE(projector));
	return tprojector;
}

float AGameGeneratedActor::DotProduct(FVector4 rhs, FVector4 lhs)
{
	return rhs[0] * lhs[0] + rhs[1] * lhs[1] + rhs[2] * lhs[2] + rhs[3] * lhs[3];
}

FVector4 AGameGeneratedActor::Normalize(FVector4 v) {
	float n = FGenericPlatformMath::Sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
	return v / n;
}

FVector AGameGeneratedActor::Project(FVector4 a, TArray<FVector> proj) {
	float x = a[0] * proj[0][0] + a[1] * proj[1][0] + a[2] * proj[2][0] + a[3] * proj[3][0];
	float y = a[0] * proj[0][1] + a[1] * proj[1][1] + a[2] * proj[2][1] + a[3] * proj[3][1];
	float z = a[0] * proj[0][2] + a[1] * proj[1][2] + a[2] * proj[2][2] + a[3] * proj[3][2];
	return FVector(x, y, z);
}

void AGameGeneratedActor::GenerateTriangles(FVector a, FVector b, FVector c, FVector d, TArray<FCustomMeshTriangle>& triangles) {
	GenerateTriangles(a, b, c, triangles);
	GenerateTriangles(b, c, d, triangles);
	GenerateTriangles(c, d, a, triangles);
	GenerateTriangles(d, a, c, triangles);
}

void AGameGeneratedActor::GenerateTriangles(FVector a, FVector b, FVector c, TArray<FCustomMeshTriangle>& triangles) {
	FCustomMeshTriangle tri;

	tri.Vertex0 = a;
	tri.Vertex1 = b;
	tri.Vertex2 = c;
	triangles.Add(tri);
	InvertTriangle(tri);
	triangles.Add(tri);
}

void AGameGeneratedActor::InvertTriangle(FCustomMeshTriangle& triangle) {
	FVector tmp = triangle.Vertex0;
	triangle.Vertex0 = triangle.Vertex2;
	triangle.Vertex2 = tmp;
}

// Called when the game starts or when spawned
void AGameGeneratedActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGameGeneratedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
	((UCustomMeshComponent*)RootComponent)->SetCustomMeshTriangles(GetTriangles(RunningTime));
}