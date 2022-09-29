#pragma once
#include "FbxModelManager.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "FbxLoader.h"
#include "DirectXCommon.h"

#include <DirectXMath.h>
#include <wrl.h>

class FbxModelObject
{
/// <summary>
/// �萔
/// </summary>
public:
	static const int MAX_BONES = 32;

/// <summary>
/// �G�C���A�X
/// </summary>
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMMATRIX = DirectX::XMMATRIX;

/// <summary>
/// �T�u�N���X�E�C���i�[�N���X
/// </summary>
public:
	class CommonFbx{
		friend class FbxModelObject;

	public:
		/// <summary>
		/// �O���t�B�b�N�p�C�v���C��������
		/// </summary>
		//void InitializeGraphicsPipeline();

		/// <summary>
		/// �f�X�g���N�^�q�[�v������
		/// </summary>
		//void InitializeDescriptorHeap();

	private:
		//DirectXCommon
		DirectXCommon* dxCommon = nullptr;

		//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelineState;
		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootSignature;

		FbxModelManager* FbxModelManager;

	};

	//�萔�o�b�t�@�p�f�[�^�\����(���W�ϊ��s��p)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;	//�r���[�v���W�F�N�V����
		XMMATRIX world;		//���[���h�s��
		Vector3 cameraPos;	//�J�������W(���[���h���W)
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public:
	/// <summary>
	/// ������
	/// </summary>
	//static void StaticInitialize(DirectXCommon* dxCommon);

	/// <summary>
	/// �ÓI�����o��@
	/// </summary>
	//static void StaticFinalize();

	//static FbxModelObject* Create();


/// <summary>
/// �����o�֐�
/// </summary>
public:

	void Initialize();

	void Update(WorldTransform worldTransform, Camera* camera);

	void Draw();

/// <summary>
/// �ÓI�����o�ϐ�
/// </summary>
private:
	//static CommonFbx* common;

/// <summary>
/// �����o�ϐ�
/// </summary>
public:
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBufferTransform;
	ComPtr<ID3D12Resource> constBufferSkin;

	//���f��
	FbxModelManager* model = nullptr;

	//�A�j���[�V����
	//1frame�̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
};

