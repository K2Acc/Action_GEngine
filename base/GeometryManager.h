#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <vector>

#include "DirectXCommon.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Vector3.h"
#include "Vector2.h"

class GeometryManager
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	template <class T> using vector = std::vector<T>;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//���_�f�[�^
	struct VertexPosNormal{
		Vector3 pos;
		Vector3 normal;
		Vector2 uv;
	};

public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(UINT texNumber);

	//Get
	const D3D12_VERTEX_BUFFER_VIEW& GetVbView()	{return vbView;}
	const D3D12_INDEX_BUFFER_VIEW& GetIbView()	{return ibView;}

private:
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuffer;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuffer;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//���_�f�[�^�z��
	vector<VertexPosNormal> vertices;
	//���_�C���f�b�N�X�z��
	vector<unsigned short> indices;

	DirectXCommon* dxCommon = nullptr;
	TextureManager* textureManager = nullptr;
};

