#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>

#include "Window.h"
#include "Vector2.h"
#include "Vector3.h"

//�萔�o�b�t�@�p�f�[�^�\��
struct ConstBufferDataViewProjection{
	DirectX::XMMATRIX view;			//���[���h -> �r���[�ϊ��s��
	DirectX::XMMATRIX projection;	//�r���[ -> �v���W�F�N�V�����ϊ��s��
	Vector3 cameraPos;				//�J�������W (���[���h���W)
};

/// <summary>
/// �r���[�v���W�F�N�V�����ϊ��f�[�^
/// </summary>
struct ViewProjection{

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffer;
	//�}�b�s���O�ς݃A�h���X
	ConstBufferDataViewProjection* constMap = nullptr;

	//�A�X�y�N�g�p
	Window* window = Window::GetInstance();

#pragma region �r���[�s��
	//���_���W
	Vector3 eye = {0,0,-50.f};
	//�����_���W
	Vector3 target = {0,0,0};
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
	/// �萔�o�b�t�@����
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// �}�b�s���O
	/// </summary>
	void Map();

	/// <summary>
	/// �s��X�V
	/// </summary>
	void UpdateMatrix();
};