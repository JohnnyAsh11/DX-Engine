#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "Vectors.h"
#include "Transform.h"

/// <summary>
/// Calculates View and Projection matrices
/// for creating perspective in the simulation.
/// </summary>
class Camera
{
private:
	float m_fFOV;
	Transform m_tTransform;
	Matrix4 m_m4View;
	Matrix4 m_m4Projection;

public:
	/// <summary>
	/// Constructs a camera with the passed in values.
	/// </summary>
	/// <param name="a_fAspectRatio">The current aspect ratio of the window.</param>
	/// <param name="a_v3StartingPosition">The starting position of the Camera.</param>
	/// <param name="a_fFOV">The Field Of View (FOV) of the Camera.</param>
	Camera(float a_fAspectRatio, Vector3 a_v3StartingPosition, float a_fFOV);

	/// <summary>
	/// Gets the view matrix.
	/// </summary>
	Matrix4 GetView(void);

	/// <summary>
	/// Gets the projection matrix.
	/// </summary>
	Matrix4 GetProjection(void);

	/// <summary>
	/// Gets the Transform of the camera.
	/// </summary>
	Transform GetTransform(void);

	/// <summary>
	/// Updates the projection matrix as needed.  Only needs to be done on window resizing.
	/// </summary>
	/// <param name="a_fAspectRatio">Window's new aspect raito.</param>
	void UpdateProjection(float a_fAspectRatio);

	/// <summary>
	/// Updates the view matrix as needed.  Needs to be called every frame.
	/// </summary>
	void UpdateView();

	/// <summary>
	/// Helper method for Camera movement.
	/// </summary>
	/// <param name="a_fDeltaTime">The change in time between frames of the simulation.</param>
	void UpdateMovement(float a_fDeltaTime);
};

#endif //__CAMERA_H_
