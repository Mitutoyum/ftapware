#pragma once

namespace FTAPWare::Math {
	struct CFrame;
	struct Vector2;
	struct Vector3;
	struct Matrix4x4;

	Vector2 WorldToScreenPoint(const Vector3& position, const Matrix4x4& viewmatrix, const Vector2& dimensions);
	CFrame LookAt(Vector3 camera, Vector3 target);
}