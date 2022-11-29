#pragma once
#include "../Engine/base/Window.h"
#include "../Engine/math/Vector/Vector3.h"

#include "../Engine/3D/ViewProjection.h"

#include <DirectXMath.h>

/// <summary>
/// �J����
/// </summary>
class Camera
{
protected:
	using XMMATRIX = DirectX::XMMATRIX;

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

	/// <summary>
	/// �����_�ړ�
	/// </summary>
	/// <param name="move">�ړ���(radian��ݒ�)</param>
	void MoveEyeVeector(Vector3 move);

	/// <summary>
	/// �J�����ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveVector(Vector3 move);

	//getter
	const XMMATRIX& GetMatProjection()	{return view.matProjection;}
	const XMMATRIX& GetMatView()	{return view.matView;}
	const XMMATRIX& GetViewProjectionMatrix()	{return view.matViewProjection;}


	const Vector3& GetEye() {return view.eye; }
	const Vector3& GetTarget() {return view.target; }
	const Vector3& GetUp() {return view.up; }
	const DirectX::XMMATRIX& GetBillboard() {return view.matBillboard;}

	//setter
	void SetEye(const Vector3& eye)	{this->view.eye = eye; }
	void SetTarget(const Vector3& target)	{this->view.target = target; }
	void SetUp(const Vector3& up)	{this->view.up = up; }

private:
	//�A�X�y�N�g�p
	Window* window;

protected:
	ViewProjection view;
};

