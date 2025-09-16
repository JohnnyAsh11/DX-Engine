#include "Camera.h"

// Including the M_PI macro
#define _USE_MATH_DEFINES
#include <math.h>

using namespace DirectX;

Camera::Camera(float a_fAspectRatio, XMFLOAT3 a_v3StartingPosition, float a_fFOV)
{
	m_fFOV = a_fFOV;
	m_tTransform = Transform();
	m_tTransform.SetPosition(a_v3StartingPosition);

	UpdateView();
	UpdateProjection(a_fAspectRatio);
}

Matrix4 Camera::GetView()
{
	return m_m4View;
}

Matrix4 Camera::GetProjection()
{
	return m_m4Projection;
}

Transform Camera::GetTransform()
{
	return m_tTransform;
}

void Camera::UpdateProjection(float a_fAspectRatio)
{
	// Creating the Projection matrix.
	XMMATRIX m4 = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFOV),				// FOV
		a_fAspectRatio,							// Aspect Ratio
		0.01f,									// Near Plane
		900.0f);								// Far Plane

	// Storing the projection matrix in the field.
	XMStoreFloat4x4(
		&m_m4Projection,
		m4);
}

void Camera::UpdateView()
{
	// Setting the up vector.
	XMFLOAT3 v3Up = m_tTransform.GetUp();
	XMVECTOR vUp = XMVectorSet(v3Up.x, v3Up.y, v3Up.z, 0.0f);

	// Setting the forward vector.
	XMFLOAT3 v3Forward = m_tTransform.GetForward();
	XMVECTOR vForward = XMVectorSet(v3Forward.x, v3Forward.y, v3Forward.z, 0.0f);

	// Creating the View matrix.
	XMMATRIX m4 = XMMatrixLookToLH(
		XMLoadFloat3(&m_tTransform.GetPosition()),		// Camera Position
		vForward,										// Camera Forward
		vUp);											// Camera Up

	// Storing the view matrix in the field.
	XMStoreFloat4x4(
		&m_m4View,
		m4);
}