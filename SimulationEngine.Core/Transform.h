#ifndef __TRANSFORM_H_
#define __TRANSFORM_H_

#include "Vectors.h"

/// <summary>
/// Calculates movement and orientation for objects in the simulation.
/// </summary>
class Transform
{
private:
	Matrix4 m_m4WorldMatrix;
	Matrix4 m_m4WorldInverseTranspose;

	Vector3 m_v3Position;
	Vector3 m_v3Scale;
	Vector3 m_v3Rotation;

	bool m_bIsDirty;

public:
	/// <summary>
	/// Constructs a Transform with base 0/identity values.
	/// </summary>
	Transform();

	// --------------------------------------------
	// Lots of methods without XML docs.
	// This is because they should be relatively 
	// self explanatory based on their name.
	// --------------------------------------------

	// Position/Rotation/Scale setters and movement methods taking in a Vector3.
	void SetPosition(Vector3 a_v3Position);
	void SetRotation(Vector3 a_v3Rotation);
	void SetScale(Vector3 a_v3Scale);
	void MoveAbsolute(Vector3 a_v3Offset);
	void MoveRelative(Vector3 a_v3Offset);
	void Rotate(Vector3 a_v3Rotation);
	void Scale(Vector3 a_v3Scale);

	// Position/Rotation/Scale setters and movement methods taking in 3 float components.
	void SetPosition(float a_fX, float a_fY, float a_fZ);
	void SetRotation(float a_fP, float a_fY, float a_fR);
	void SetScale(float a_fX, float a_fY, float a_fZ);

	void MoveAbsolute(float a_fX, float a_fY, float a_fZ);
	void MoveRelative(float a_fX, float a_fY, float a_fZ);
	void Rotate(float a_fP, float a_fY, float a_fR);
	void Scale(float a_fX, float a_fY, float a_fZ);

	// Getter for the position.
	Vector3& GetPosition(void);

	// Getter for the rotation.
	Vector3& GetRotation(void);

	// Getter for the scale.
	Vector3& GetScale(void);

	// Up direction getter.
	Vector3 GetUp(void);

	// Right direction getter.
	Vector3 GetRight(void);

	// Forward direction getter.
	Vector3 GetForward(void);

	// Gets the world matrix.
	Matrix4 GetWorld(void);

	// Gets the World InvTra matrix.
	Matrix4 GetWorldInvTra(void);

private:
	/// <summary>
	/// Calculates the World and World InvTra matrices.
	/// </summary>
	void CalculateMatrices(void);
};

#endif //__TRANSFORM_H_
