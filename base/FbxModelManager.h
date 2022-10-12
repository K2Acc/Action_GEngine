#pragma once

#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Window.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

#include "Vector2.h"
#include "Vector3.h"

struct Node{
	std::string name;
	DirectX::XMVECTOR scaling = {1,1,1,0};
	DirectX::XMVECTOR rotation = {0,0,0,0};
	DirectX::XMVECTOR translation = {0,0,0,1};
	DirectX::XMMATRIX transform;
	DirectX::XMMATRIX globalTransform;
	Node* parent = nullptr;
};

class FbxModelManager{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	template <class T> using vector = std::vector<T>;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	using string = std::string;

public:
	friend class FbxLoader;
public:
	//���_�f�[�^
	struct VertexPosNormalUv{
		Vector3 pos;
		Vector3 normal;
		Vector2 uv;
	};

public:

	//�o�b�t�@����
	void CreateBuffer(ID3D12Device* device);

	//�`��
	void Draw(ID3D12GraphicsCommandList* commandList);

	const XMMATRIX& GetModelTransform() {return meshNode->globalTransform;}

private:
	string name;
	vector<Node> nodes;

	//���b�V�������m�[�h
	Node* meshNode = nullptr;
	//���_�f�[�^�z��
	vector<VertexPosNormalUv> vertices;
	//���_�C���f�b�N�X
	vector<unsigned short> indices;

	ComPtr<ID3D12Resource> vertBuffer;
	ComPtr<ID3D12Resource> indexBuffer;
	ComPtr<ID3D12Resource> texBuffer;

	VertexPosNormalUv* vertMap = nullptr;
	unsigned short* indexMap = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView{};

	ComPtr<ID3D12DescriptorHeap> descriptorHeapSRV;


	Vector3 ambient = {1,1,1};
	Vector3 diffuse = {1,1,1};
	TexMetadata metaData = {};
	ScratchImage scratchImage = {};
};