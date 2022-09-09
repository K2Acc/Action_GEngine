#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>
#include<wrl.h>
#include <string>

#include "Window.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "Vector2.h"
#include "Vector3.h"

/// <summary>
/// �X�v���C�g
/// </summary>
class Sprite
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	class Common{
		friend class Sprite;

	private:
		DirectXCommon* dxCommon = nullptr;
		//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelinestate;
		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootsignature;
		//�ˉe�s��
		DirectX::XMMATRIX matProjection{};
		//�e�N�X�`���}�l�[�W���[
		TextureManager* textureManager = nullptr;

	public:
		void InitializeGraphicsPipeline();
	};

	//���_�f�[�^
	struct VertexPosUv{
		Vector3 pos;
		Vector2 uv;
	};

	//�萔�o�b�t�@�p
	struct ConstBufferData{
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX mat;
	};

public:

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	/// <param name="directoryPath">�V�F�[�_�[�p�X</param>
	static void StaticInitialize(ID3D12Device* device, 
		int window_width, int window_height,
		const std::wstring& directoryPath = L"Resources/");

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="commandList">�R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="textureNumber">�ԍ�</param>
	/// <param name="pos">���W</param>
	/// <param name="color">�F</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="isFlipX">���E���]</param>
	/// <param name="isFlipY">�㉺���]</param>
	/// <returns>�X�v���C�g</returns>
	static Sprite* Create(uint32_t textureNumber, Vector2 pos, DirectX::XMFLOAT4 color = {1,1,1,1},
		Vector2 anchorpoint = {0.f,0.f}, bool isFlipX = false, bool isFlipY = false);

private:

	//���_��
	static const int VertNum = 4;

	//�T�u�N���X(���ʏ���)
	static Common* common;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Sprite();
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="textureNumber">�ԍ�</param>
	/// <param name="pos">���W</param>
	/// <param name="size">�T�C�Y</param>
	/// <param name="color">�F</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="isFlipX">���E���]</param>
	/// <param name="isFlipY">�㉺���]</param>
	Sprite(uint32_t textureNumber, Vector2 pos, Vector2 size, DirectX::XMFLOAT4 color,
		Vector2 anchorpoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>bool</returns>
	bool Initialize();

	/// <summary>
	/// �e�N�X�`���ԍ��ݒ�
	/// </summary>
	/// <param name="textureNumber">�e�N�X�`���ԍ�</param>
	void SetTextureHandle(uint32_t textureNumber);

	uint32_t GetTextureHandle()	{return textureHandle;}

private:
	ComPtr<ID3D12Resource> vertBuffer;
	ComPtr<ID3D12Resource> constBuffer;

	VertexPosUv* vertexMap = nullptr;
	ConstBufferData* constMap = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vbView{};

	uint32_t textureHandle;
	float rotation = 0;
	Vector2 position{};
	Vector2 size = {100.f,100.f};
	Vector2 anchorPoint = {0.f,0.f};
	DirectX::XMMATRIX matWorld{};
	DirectX::XMFLOAT4 color = {1,1,1,1};
	bool isFlipX = false;
	bool isFlipY = false;
	Vector2 texBase = {0.f,0.f};
	Vector2 texSize = {100.f,100.f};
	D3D12_RESOURCE_DESC resourceDesc;

private:
	void TransferVertices();
};

