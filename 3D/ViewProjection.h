#pragma once

#include <DirectXMath.h>

#include "Window.h"
#include "Vector3.h"


/// <summary>
/// �r���[�v���W�F�N�V�����ϊ��f�[�^
/// </summary>
struct ViewProjection{


	//�A�X�y�N�g�p
	Window* window = Window::GetInstance();

#pragma region �r���[�s��
	//���_���W
	Vector3 eye = {0,0,-100.f};
	//�����_���W
	Vector3 target = {0,20,0};
	//������x�N�g��
	Vector3 up = {0,1,0};
#pragma endregion

#pragma region �ˉe�ϊ��s��
	//������������p
	float fovAngleY = DirectX::XMConvertToRadians(45.f);
	//�r���[�|�[�g�̃A�X�y�N�g��
	float aspectRatio = (float)window->GetWindowWidth()/(float)window->GetWindowHeight();
	//�[�x���E(��O)
	float nearZ = 0.1f;
	//�[�x���E(��)
	float farZ = 1000.f;
#pragma endregion

	//�r���[�s��
	DirectX::XMMATRIX matView;
	//�ˉe�s��
	DirectX::XMMATRIX matProjection;

	
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �s��X�V
	/// </summary>
	void UpdateMatrix();
};