#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <DirectXMath.h>

#include"TextureManager.h"
#include "Vector2.h"
#include "Vector3.h"

/// <summary>
/// �X�v���C�g
/// </summary>
class Sprite
{
public://�G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public://�T�u�N���X
	class Common{
		friend class Sprite;

	private:
		DirectXCommon* dxCommon = nullptr;
		//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelinestate;
		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootsignature;
		//�ˉe�s��
		XMMATRIX matProjection{};
		//�e�N�X�`���}�l�[�W���[
		TextureManager* textureManager = nullptr;

	public:
		void InitializeGraphicsPipeline();
	};

	//�X�v���C�g�f�[�^�\��
	struct VertexPosUv
	{
		Vector3 pos;
		Vector2 uv;
	};

	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��
	struct ConstBufferData{
		XMMATRIX mat;	//3D�ϊ��s��
		XMFLOAT4 color;	//�F(RGBA)
	};

public://�ÓI�����o�֐�
	/// <summary>
	/// �ÓI�����o�̏�����
	/// </summary>
	static void StaticInitialize(DirectXCommon* dxCommon, TextureManager* texManager, int window_width, int window_height);

	/// <summary>
	/// �ÓI�����o�̉��
	/// </summary>
	static void StaticFinalize();

	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C���̃Z�b�g
	/// </summary>
	/// <param name="commandList">�R�}���h���X�g</param>
	static void SetPipelineState(ID3D12GraphicsCommandList* commandList);



private://�ÓI�����o�ϐ�
	static Common* common;


public://�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(UINT texNumber);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* commandList);

	//�X�v���C�g�P�̒��_�o�b�t�@�̓]��
	void SpriteTransferVertexBuffer();


	/// <summary>
	/// ���W�擾
	/// </summary>
	Vector2 GetPosition();

	/// <summary>
	/// ���W�ݒ�
	/// </summary>
	void SetPosition(Vector2 pos);

	/// <summary>
	/// �T�C�Y�擾
	/// </summary>
	Vector2 GetSize();

	/// <summary>
	/// �T�C�Y�ݒ�
	/// </summary>
	void SetSize(Vector2 size);

	/// <summary>
	/// �A���J�[�|�C���g�ݒ�
	/// </summary>
	void SetAnchorpoint(Vector2 pos);

	/// <summary>
	/// �؂蔲���T�C�Y�ݒ�
	/// </summary>
	/// <param name="tex_x">����X</param>
	/// <param name="tex_y">����Y</param>
	/// <param name="tex_width">��</param>
	/// <param name="tex_height">����</param>
	void SetTextureRect(float tex_x, float tex_y, float tex_width, float tex_height);

	/// <summary>
	/// ���E���]�ݒ�
	/// </summary>
	void SetIsFlipX(bool IsFlipX);

	/// <summary>
	/// �㉺���]�ݒ�
	/// </summary>
	void SetIsFlipY(bool IsFlipY);

	/// <summary>
	/// �F�ݒ�
	/// </summary>
	void SetColor(XMFLOAT4 color);

private://�����o�ϐ�
	///���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	///���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffData;
	//Z������̉�]�p
	float rotation = 0.f;
	//���W
	Vector3 position = {0, 0, 0};
	//���[���h�s��
	XMMATRIX matWorld;
	//�F
	XMFLOAT4 color = {1, 1, 1, 1};
	//�e�N�X�`���ԍ�
	UINT texNumber = 0;
	//�傫��
	Vector2 size = {100, 100};
	//�A���J�[�|�C���g
	Vector2 anchorpoint = {0.0f, 0.0f};
	//���E���]
	bool IsFlipX = false;
	//�㉺���]
	bool IsFlipY = false;
	//�e�N�X�`��������W
	Vector2 texLeftTop = {0,0};
	//�e�N�X�`���؂�o���T�C�Y
	Vector2 texSize = {100, 100};
	//��\��
	bool IsInvisible = false;
};

