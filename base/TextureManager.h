#pragma once

#include "DirectXCommon.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include "DirectXTex.h"

using namespace DirectX;

//�e�N�X�`���}�l�[�W���[
class TextureManager
{
public://�G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:	//�萔
	//�e�N�X�`���̍ő喇��
	static const int maxTextureCount = 512;

public:	//�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="dxCommon">DirectXCommon->device.Get()</param>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="texnumber">�e�N�X�`���ԍ�</param>
	/// <param name="filename">�e�N�X�`���t�@�C����</param>
	void LoadTexture(UINT texnumber, const wchar_t* filename);

	/// <summary>
	/// �e�N�X�`���o�b�t�@�擾
	/// </summary>
	/// <param name="texnumber">�e�N�X�`���ԍ�</param>
	/// <returns></returns>
	ID3D12Resource* GetSpriteTexBuffer(UINT texnumber);

	/// <summary>
	/// �f�X�N���v�^�q�[�v���Z�b�g
	/// </summary>
	/// <param name="commandList">�O���t�B�b�N�X�R�}���h</param>
	void SetDescriptorHeaps (ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// SRV���Z�b�g
	/// </summary>
	/// <param name="commandList">�O���t�B�b�N�X�R�}���h</param>
	/// <param name="RootParameterIndex">���[�g�p�����[�^�ԍ�</param>
	/// <param name="texnumber">�e�N�X�`���ԍ�</param>
	void SetShaderResourceView(ID3D12GraphicsCommandList* commandList, UINT RootParameterIndex, UINT texnumber);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�擾
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetDescHeap() {return descHeap.Get(); }

private:	//�����o�ϐ�
	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap> descHeap;
	//�e�N�X�`�����\�[�X(�e�N�X�`���o�b�t�@)�̔z��
	ComPtr<ID3D12Resource> textureBuffer[maxTextureCount];

	DirectXCommon* dxCommon = nullptr;
};

