#pragma once
#include "Window.h"
#include "Vector3.h"

#include <DirectXMath.h>

/// <summary>
/// �J����
/// </summary>
class Camera
{
public:
	using XMMATRIX = DirectX::XMMATRIX;
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:
	static Camera* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	void UpdateViewMatrix();

	void UpdateProjectionMatrix();


	//getter
	const XMMATRIX& GetMatProjection()	{return matProjection;}
	const XMMATRIX& GetMatView()	{return matView;}
	const XMMATRIX& GetViewProjectionMatrix()	{return matViewProjection;}


	const Vector3& GetEye() {return eye; }
	const Vector3& GetTarget() {return target; }
	const Vector3& GetUp() {return up; }

	//setter
	void SetEye(const Vector3& eye)	{this->eye = eye; }
	void SetTarget(const Vector3& target)	{this->target = target; }
	void SetUp(const Vector3& up)	{this->up = up; }

	void CameraMovement(XMFLOAT3 pos);

private:
	//�A�X�y�N�g�p
	Window* window;

protected:

	//�������e
	XMMATRIX matProjection;	//�v���W�F�N�V�����s��
	//�r���[�ϊ��s��
	XMMATRIX matView;		//�r���[�s��
	Vector3 eye;			//���_���W
	Vector3 target;		//�����_���W
	Vector3 up;			//������x�N�g��

	XMMATRIX matViewProjection;
};

