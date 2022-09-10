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
	public:
		void InitializeGraphicsPipeline(const std::wstring &directoryPath);
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
	static void StaticInitialize(DirectXCommon* dxCommon,
		int window_width, int window_height,
		const std::wstring& directoryPath = L"Resources/");

	/// <summary>
	/// �ÓI��@
	/// </summary>
	static void StaticFinalize();

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

	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* commandList;

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
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���W�ݒ�
	/// </summary>
	void SetPosition(Vector2 pos);
	Vector2 GetPosition()	{return position;}

	/// <summary>
	/// �p�x�̐ݒ�
	/// </summary>
	/// <param name="rotation">�p�x</param>
	void SetRotation(float rotation);
	float GetRotation() { return rotation; }

	/// <summary>
	/// �T�C�Y�ݒ�
	/// </summary>
	void SetSize(Vector2 size);
	Vector2 GetSize()	{return size;}

	/// <summary>
	/// �A���J�[�|�C���g�ݒ�
	/// </summary>
	void SetAnchorpoint(Vector2 pos);
	Vector2 GetAnchorpoint()	{return anchorPoint;}

	/// <summary>
	/// �e�N�X�`���g���~���O
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
	bool GetIsFlipX()	{return isFlipX;}

	/// <summary>
	/// �㉺���]�ݒ�
	/// </summary>
	void SetIsFlipY(bool IsFlipY);
	bool GetIsFlipY()	{return isFlipY;}

	/// <summary>
	/// �F�ݒ�
	/// </summary>
	void SetColor(DirectX::XMFLOAT4 color);
	DirectX::XMFLOAT4 GetColor()	{return color;}

private:
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuffer;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffer;
	//���_�o�b�t�@�}�b�v
	VertexPosUv* vertexMap = nullptr;
	//�萔�o�b�t�@�}�b�v
	ConstBufferData* constMap = nullptr;

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	
	//�e�N�X�`���n���h��
	uint32_t textureHandle;
	//��]�p
	float rotation = 0;
	//���W
	Vector2 position{};
	//�T�C�Y
	Vector2 size = {100.f,100.f};
	//�A���J�[�|�C���g
	Vector2 anchorPoint = {0.f,0.f};
	//���[���h���W
	DirectX::XMMATRIX matWorld{};
	//�F
	DirectX::XMFLOAT4 color = {1,1,1,1};
	//���E���]
	bool isFlipX = false;
	//�㉺���]
	bool isFlipY = false;
	//�؂蔲��������W
	Vector2 texBase = {0.f,0.f};
	//�؂蔲���T�C�Y
	Vector2 texSize = {100.f,100.f};
	//��\��
	bool IsInvisible = false;
	//���\�[�X�ݒ�
	//D3D12_RESOURCE_DESC resourceDesc;

private:
	//���_���]��
	void TransferVertices();
};

