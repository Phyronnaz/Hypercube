// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Hypercube.h"
#include "GameGeneratedActor.h"
#include "DrawDebugHelpers.h"
#include "math.h"

AGameGeneratedActor::AGameGeneratedActor(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;

	UCustomMeshComponent* mesh = PCIP.CreateDefaultSubobject<UCustomMeshComponent>(this, TEXT("GeneratedMesh"));

	mesh->SetCustomMeshTriangles(GetTriangles(RunningTime));

	RootComponent = mesh;
}

TArray<FCustomMeshTriangle> AGameGeneratedActor::GetTriangles(float time) {
	TArray<FCustomMeshTriangle> triangles;

	FVector4 planeNormal = FVector4(sin(theta1)*sin(theta2)*sin(theta3),
		sin(theta1)*sin(theta2)*cos(theta3),
		sin(theta1)*cos(theta2),
		cos(theta1)*cos(theta2));

	TArray<FVector> projector = GetProjector(planeNormal);
	projector.Reset(4);
	FVector p[] = { FVector(1, 0, 0), FVector(0, 1, 0) , FVector(0, 0, 1) , FVector(0, 0, 0) };
	projector.Append(p, ARRAYSIZE(p));

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
							v[l] = d * c;

							float angle = time / 5;

							if (use_YZ)
								YZ_angle = angle;
							if (use_XZ)
								XZ_angle = angle;
							if (use_XY)
								XY_angle = angle;
							if (use_XW)
								XW_angle = angle;
							if (use_YW)
								YW_angle = angle;
							if (use_ZW)
								ZW_angle = angle;

							FVector4 YZ[] = { FVector4(1, 0, 0, 0),
											  FVector4(0, cos(YZ_angle), sin(YZ_angle), 0),
											  FVector4(0, -sin(YZ_angle), cos(YZ_angle), 0),
											  FVector4(0, 0, 0, 1) };

							FVector4 XZ[] = { FVector4(cos(XZ_angle), 0, -sin(XZ_angle), 0),
											  FVector4(0, 1, 0, 0),
											  FVector4(sin(XZ_angle), 0, cos(XZ_angle), 0),
											  FVector4(0, 0, 0, 1) };

							FVector4 XY[] = { FVector4(cos(XY_angle), sin(XY_angle), 0, 0),
											  FVector4(-sin(XY_angle), cos(XY_angle), 0, 0),
											  FVector4(0, 0, 1, 0),
											  FVector4(0, 0, 0, 1) };

							FVector4 XW[] = { FVector4(cos(XW_angle), 0, 0, -sin(XW_angle)),
											  FVector4(0, 1, 0, 0),
											  FVector4(0, 0, 1, 0),
											  FVector4(sin(XW_angle), 0, 0, cos(XW_angle)) };

							FVector4 YW[] = { FVector4(1, 0, 0, 0),
											  FVector4(0, cos(YW_angle), 0, sin(YW_angle)),
											  FVector4(0, 0, 1, 0),
											  FVector4(0, -sin(YW_angle), 0, cos(YW_angle)) };

							FVector4 ZW[] = { FVector4(1, 0, 0, 0),
											  FVector4(0, 1, 0, 0),
											  FVector4(0, 0, cos(ZW_angle), sin(ZW_angle)),
											  FVector4(0, 0, -sin(ZW_angle), cos(ZW_angle)) };

							v = Rotate(v, YZ);
							v = Rotate(v, XZ);
							v = Rotate(v, XY);
							v = Rotate(v, XW);
							v = Rotate(v, YW);
							v = Rotate(v, ZW);

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
	FVector4 basis[] = { FVector4(planeNormal[0], 0.f, 0.f, 0) ,
						 FVector4(0.f, planeNormal[1], 0.f, 0) ,
						 FVector4(0.f, 0.f, planeNormal[2], 0) };
	if (planeNormal[3] != 0)
	{
		basis[0][3] = -planeNormal[0] / planeNormal[3];
		basis[1][3] = -planeNormal[1] / planeNormal[3];
		basis[2][3] = -planeNormal[2] / planeNormal[3];
	}
	basis[0] = Normalize(basis[0]);

	basis[1] += -basis[0] * DotProduct(basis[0], basis[1]);
	basis[1] = Normalize(basis[1]);

	basis[2] += -basis[0] * DotProduct(basis[0], basis[2]);
	basis[2] += -basis[1] * DotProduct(basis[1], basis[2]);
	basis[2] = Normalize(basis[2]);
	FVector projector[] = { FVector(basis[0][0], basis[1][0], basis[2][0]),
							FVector(basis[0][1], basis[1][1], basis[2][1]),
							FVector(basis[0][2], basis[1][2], basis[2][2]),
							FVector(basis[0][3], basis[1][3], basis[2][3]) };
	TArray<FVector> tprojector;
	tprojector.Append(projector, ARRAYSIZE(projector));
	return tprojector;
}

FVector4 AGameGeneratedActor::Rotate(FVector4 v, FVector4* m) {
	FVector4 r;
	for (int i = 0; i < 4; i++) {
		r[i] = v[0] * m[0][i] + v[1] * m[1][i] + v[2] * m[2][i] + v[3] * m[3][i];
	}
	return r;
}

float AGameGeneratedActor::DotProduct(FVector4 rhs, FVector4 lhs)
{
	return rhs[0] * lhs[0] + rhs[1] * lhs[1] + rhs[2] * lhs[2] + rhs[3] * lhs[3];
}

FVector4 AGameGeneratedActor::Normalize(FVector4 v) {
	float n = FGenericPlatformMath::Sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
	if (n == 0) {
		return FVector4(0, 0, 0, 0);
	}
	else
	{
		return v / n;
	}
}

FVector AGameGeneratedActor::Project(FVector4 a, TArray<FVector> proj) {
	float x = a[0] * proj[0][0] + a[1] * proj[1][0] + a[2] * proj[2][0] + a[3] * proj[3][0];
	float y = a[0] * proj[0][1] + a[1] * proj[1][1] + a[2] * proj[2][1] + a[3] * proj[3][1];
	float z = a[0] * proj[0][2] + a[1] * proj[1][2] + a[2] * proj[2][2] + a[3] * proj[3][2];
	return FVector(x, y, z);
}

void AGameGeneratedActor::GenerateTriangles(FVector a, FVector b, FVector c, FVector d, TArray<FCustomMeshTriangle>& triangles) {
	FVector aa = a * (AActor::GetActorScale() + 0.5f) + AActor::GetActorLocation();
	FVector bb = b * (AActor::GetActorScale() + 0.5f) + AActor::GetActorLocation();
	FVector cc = c * (AActor::GetActorScale() + 0.5f) + AActor::GetActorLocation();
	FVector dd = d * (AActor::GetActorScale() + 0.5f) + AActor::GetActorLocation();


	DrawDebugLine(GetWorld(), aa, bb, FColor::Black, false, -1, 0, 0.75f);
	DrawDebugLine(GetWorld(), bb, cc, FColor::Black, false, -1, 0, 0.75f);
	DrawDebugLine(GetWorld(), cc, dd, FColor::Black, false, -1, 0, 0.75f);
	DrawDebugLine(GetWorld(), dd, aa, FColor::Black, false, -1, 0, 0.75f);

	GenerateTriangles(a, b, c, triangles);
	GenerateTriangles(c, d, a, triangles);
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

void AGameGeneratedActor::SetMaterial(UMaterialInterface* Material) {
	((UCustomMeshComponent*)RootComponent)->SetMaterial(0, Material);
}