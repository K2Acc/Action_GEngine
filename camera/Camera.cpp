#include "Camera.h"

Camera *Camera::GetInstance()
{
	static Camera instance;
	return &instance;
}

void Camera::Initialize()
{
	window = Window::GetInstance();

	const float distance = 20.f;	//�J�����̋���

	//�J����
	view.eye = {0, 0, -distance};
	view.target = {0, 0, 0};
	view.up = {0, 1, 0};


	view.UpdateViewMatrix();
	view.UpdateProjectionMatrix(window->GetWindowWidth(),window->GetWindowHeight());
}

void Camera::Update()
{
	view.matViewProjection = view.matView * view.matProjection;

	view.UpdateViewMatrix();
	view.UpdateProjectionMatrix(window->GetWindowWidth(),window->GetWindowHeight());
}

void Camera::MoveEyeVeector(Vector3 move)
{
	Vector3 eye_= GetEye();
	eye_ += move;
	SetEye(eye_);
}

void Camera::MoveVector(Vector3 move)
{
	Vector3 eye_ = GetEye();
	Vector3 target_ = GetTarget();
	eye_ += move;
	target_ += move;
	SetEye(eye_);
	SetTarget(target_);
}


