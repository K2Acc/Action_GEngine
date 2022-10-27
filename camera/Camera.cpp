#include "Camera.h"

using namespace DirectX;

Camera *Camera::GetInstance()
{
	static Camera instance;
	return &instance;
}

void Camera::Initialize()
{
	window = Window::GetInstance();

	const float distance = 100.f;	//JÌ£

	//J
	eye = {0, 0, -distance};
	target = {0, 0, 0};
	up = {0, 1, 0};


	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::Update()
{
	matViewProjection = matView * matProjection;

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	//r[Ï·sñ
	XMFLOAT3 leye = {eye.x,eye.y,eye.z};
	XMFLOAT3 ltarget = {target.x,target.y,target.z};
	XMFLOAT3 lup = {up.x,up.y,up.z};

	matView = XMMatrixLookAtLH(XMLoadFloat3(&leye), XMLoadFloat3(&ltarget), XMLoadFloat3(&lup));	
}

void Camera::UpdateProjectionMatrix()
{
	//§e
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),	//ãºæp45
		(float)window->GetWindowWidth() / window->GetWindowHeight(),			//aspectä(æÊ¡/æÊc)
		0.1f, 1000.0f				//O[A[
	);
}
