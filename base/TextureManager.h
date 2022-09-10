#pragma once
#include "DirectXCommon.h"
#include "DirectXTex.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <string>
#include <array>


/// <summary>
/// �e�N�X�`���}�l�[�W���[
/// </summary>
class TextureManager
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using string = std::string;
public:
	//�e�N�X�`������(�f�X�N���v�^��)
	static const size_t maxTextureCount = 512;

	/// <summary>
	/// �e�N�X�`��
	/// </summary>
	struct Texture {
		// �e�N�X�`�����\�[�X
		ComPtr<ID3D12Resource> resource;
		// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		// ���O
		string name;
	};

public:

	/// <summary>
	/// �V���O���g��
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();

	static void Load(uint32_t number, const string& fileName);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="dxCommon">DirectXCommon ���ʏ���</param>
	void Initialize(DirectXCommon* dxCommon, string directoryPath = "Resources/");

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
	/// ���\�[�X���擾
	/// </summary>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	/// <returns>���\�[�X���</returns>
	const D3D12_RESOURCE_DESC GetResoureDesc(uint32_t number);

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
	std::array<Texture, maxTextureCount> textures;
	//�f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize = 0u;
	//���̃f�X�N���v�^�q�[�v�ԍ�
	uint32_t indexNextDescriptorHeap = 0u;

	//�f�B���N�g���p�X
	string directoryPath;

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="number">�ԍ�</param>
	/// <param name="filename">�t�@�C����</param>
	void LoadTexture(uint32_t number, const string filename);
};

