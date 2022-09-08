#pragma once
#include "DirectXCommon.h"
#include "DirectXTex.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <string>

/// <summary>
/// �e�N�X�`��
/// </summary>
class TextureManager
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//�e�N�X�`������(�f�X�N���v�^��)
	static const int maxTextureCount = 512;
	using string = std::string;

	/// <summary>
	/// �e�N�X�`��
	/// </summary>
	struct Texture {
		// �e�N�X�`�����\�[�X
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		// ���O
		std::string name;
	};

public:

	/// <summary>
	/// �V���O���g��
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="dxCommon">DirectXCommon ���ʏ���</param>
	void Initialize(DirectXCommon* dxCommon, string directoryPath = "Resources/");

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="number">�ԍ�</param>
	/// <param name="filename">�t�@�C����</param>
	void LoadTexture(uint32_t number, const string filename);

	/// <summary>
	/// �S�e�N�X�`���N���A
	/// </summary>
	void ResetAll();

	///Getter
	
	/// <summary>
	/// �e�N�X�`���o�b�t�@�擾
	/// </summary>
	/// <param name="number">�ԍ�</param>
	/// <returns></returns>
	ID3D12Resource* GetTextureBuffer(uint32_t number);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�擾
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetDescriptorHeap();


	///Setter

	/// <summary>
	/// �f�X�N���v�^�q�[�v�Z�b�g
	/// </summary>
	/// <param name="commandList">�R�}���h���X�g</param>
	void SetDescriptorHeaps(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// SRV���Z�b�g
	/// </summary>
	/// <param name="commandList">�R�}���h���X�g</param>
	/// <param name="RootParameterIndex">���[�g�p�����[�^�ԍ�</param>
	/// <param name="texnumber">�e�N�X�`���ԍ�</param>
	void SetShaderResourceView(ID3D12GraphicsCommandList* commandList, UINT RootParameterIndex, uint32_t texnumber);


private:
	DirectXCommon* dxCommon = nullptr;

	//�e�N�X�`���p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	//�e�N�X�`���o�b�t�@�̔z��
	ComPtr<Texture> textureBuffer[maxTextureCount];
	//�f�B���N�g���p�X
	string directoryPath;
};

