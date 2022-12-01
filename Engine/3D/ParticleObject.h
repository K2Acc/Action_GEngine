#pragma once
#include <d3d12.h>
#include <wrl.h>

#include "../Engine/base/DirectXCommon.h"
#include "../Engine/base/ParticleManager.h"
#include "../Engine/base/TextureManager.h"
#include "Camera.h"
#include "WorldTransform.h"

/// <summary>
/// �􉽊w�I�u�W�F�N�g
/// </summary>
class ParticleObject
{
/// <summary>
/// �G�C���A�X
/// </summary>
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

/// <summary>
/// �C���i�[�N���X
/// </summary>
public:
	class CommonGeometry{
		friend class ParticleObject;

	public:
		/// <summary>
		/// �O���t�B�b�N�p�C�v���C��������
		/// </summary>
		void InitializeGraphicsPipeline();

		/// <summary>
		/// �f�X�g���N�^�q�[�v������
		/// </summary>
		void InitializeDescriptorHeap();

	private:
		//DirectXCommon
		DirectXCommon* dxCommon = nullptr;

		//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelineState;
		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootSignature;
		//�f�X�N���v�^�q�[�v(�萔�o�b�t�@�r���[�p)
		ComPtr<ID3D12DescriptorHeap> basicDescHeap;

		ParticleManager* particleManager;
		TextureManager* textureManager;

	};

	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��
	struct ConstBufferData{
		DirectX::XMMATRIX mat;	//3D�ϊ��s��
		DirectX::XMMATRIX matBillboard;	//�r���{�[�h�s��
	};

public:
	/// <summary>
	/// ������
	/// </summary>
	static void StaticInitialize(DirectXCommon* dxCommon);

	/// <summary>
	/// �ÓI�����o��@
	/// </summary>
	static void StaticFinalize();

	static ParticleObject* Create(UINT texNumber, DirectX::XMFLOAT4 color = {1,1,1,1});

private:
	static CommonGeometry* common;

public:
	ParticleObject();
	ParticleObject(UINT texNumber, DirectX::XMFLOAT4 color = {1,1,1,1});

	/// <summary>
	/// ������
	/// </summary>
	bool Initialize(UINT texNumber);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(WorldTransform worldTransform, Camera* camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �p�[�e�B�N���ǉ�
	/// </summary>
	/// <param name="life">��������</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	/// <param name="accel">�����x</param>
	/// <param name="start_scale">�J�n���X�P�[��</param>
	/// <param name="end_scale">�I�����X�P�[��</param>
	void Add(int life, Vector3 position, Vector3 velocity, Vector3 accel, float start_scale, float end_scale);
private:
	//�e�N�X�`���f�[�^
	UINT texNumber = 0;

	//�萔�o�b�t�@(�s��)
	ComPtr<ID3D12Resource> constBuffer;
	//�}�b�s���O�p�|�C���^
	ConstBufferData* constMap = nullptr;

	XMFLOAT4 color = {1,1,1,1};
};
