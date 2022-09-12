#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>

#include "GeometryManager.h"
#include "DirectXCommon.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class GeometryModel
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:
	static const int maxObjectCount = 512;

public:
	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��
	struct ConstBufferData{
		DirectX::XMMATRIX mat;	//3D�ϊ��s��
		XMFLOAT4 color;	//�F(RGBA)
	};

private:
	//���ʃf�[�^
	class Common
	{
		friend class GeometryModel;
		
	public:
		/// <summary>
		/// �O���t�B�b�N�X�p�C�v���C��������
		/// </summary>
		void InitializeGraphicsPipeline();

		/// <summary>
		/// �f�X�N���v�^�q�[�v������
		/// </summary>
		void InitializeDescriptorHeap();

		/// <summary>
		/// �J�����̏�����
		/// </summary>
		//void InitializeCamera();

	private:
		//DirectX12�x�[�X
		DirectXCommon* dxCommon = nullptr;
		//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelinestate;
		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootsignature;
		//�f�X�N���v�^�q�[�v(�萔�o�b�t�@�r���[�p)
		ComPtr<ID3D12DescriptorHeap> basicDescHeap;
		//���Ɏg���f�X�N���v�^�q�[�v�̔ԍ�
		int descHeapIndex = 0;
		//���f���f�[�^
		GeometryManager* model = nullptr;
	};

/// <summary>
/// �ÓI�����o�֐�
/// </summary>
public:
	/// <summary>
	/// ������
	/// </summary>
	static void StaticInitialize(GeometryManager* model);

	/// <summary>
	/// �f�X�N���v�^�q�[�v���Z�b�g
	/// </summary>
	static void ResetDescriptorHeap();

	/// <summary>
	/// ���
	/// </summary>
	static void StaticFinalize();

/// <summary>
/// �ÓI�����o�ϐ�
/// </summary>
private:
	static Common* common;

/// <summary>
/// �����o�֐�
/// </summary>
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(const WorldTransform& worldTransform, const ViewProjection& viewProjection);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="commandList">�O���t�B�b�N�X�R�}���h</param>
	void Draw(ID3D12GraphicsCommandList* commandList);



	//setter
	void SetTexNumber(UINT texNumber)	{this->texNumber = texNumber;	}

	void SetColor(const XMFLOAT4& color)	{this->color = color; }

	/*void SetScale(const Vector3& scale)	{this->scale = scale; }

	void SetRotation(const Vector3& rotation)	{this->rotation = rotation; }

	void SetPosition(const Vector3& position)	{this->position = position; }*/


	//getter
	/*const XMFLOAT4& GetColor()	{return this->color; }
	const Vector3& GetScale() {return this->scale; }
	const Vector3& GetRotation()	{return this->rotation; }
	const Vector3& GetPosition()	{return this->position; }

	const XMMATRIX& GetWorld()	{return this->matWorld;}*/

/// <summary>
/// �����o�ϐ�
/// </summary>
private:

	UINT texNumber = 1;

	//�萔�o�b�t�@(�s��p)
	ComPtr<ID3D12Resource> constBuff;
	//�}�b�s���O�p�|�C���^
	ConstBufferData* constBuffer = nullptr;

	XMFLOAT4 color = {1,1,1,1};

	////�萔�o�b�t�@�r���[��CPU�A�h���X
	//CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV;
	////�萔�o�b�t�@�r���[��GPU�A�h���X
	//CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;

	//�g�����X�t�H�[��
	////�A�t�B���ϊ�
	//Vector3 scale = {1.0f, 1.0f, 1.0f};
	//Vector3 rotation = {0.0f, 0.0f, 0.0f};
	//Vector3 position = {0.0f, 0.0f, 0.0f};
	////���[���h�ϊ��s��
	//DirectX::XMMATRIX matWorld;
	////�e�I�u�W�F�N�g�ւ̃|�C���^
	//GeometryModel* parent = nullptr;
};

