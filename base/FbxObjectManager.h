#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <DirectXTex.h>

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

#include <fbxsdk.h>

#include "Vector2.h"
#include "Vector3.h"

struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = {1, 1, 1, 0};
	//���[�J����]�p
	DirectX::XMVECTOR rotation = {0,0, 0, 0};
	//���[�J���ړ�
	DirectX::XMVECTOR translation = {0, 0, 0, 1};
	//���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;
	//�e�m�[�h
	Node* parent = nullptr;
};


//�{�[���\����
struct Bone
{
	//���O
	std::string name;
	//�����p���̋t�s��
	DirectX::XMMATRIX invInitialPose;
	//�N���X�^�[(FBX���̃{�[�����)
	FbxCluster* fbxCluster;
	//�R���X�g���N�^
	Bone(const std::string& name)
	{
		this->name = name;
	}
};

/// <summary>
/// FBX���f��
/// </summary>
class FbxObjectManager
{
/// <summary>
/// �萔
/// </summary>
public:
	//�{�[���C���f�b�N�X�̍ő吔
	static const int MAX_BONE_INDICES = 4;

/// <summary>
/// �G�C���A�X
/// </summary>
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public:
	//�t�����h�N���X
	friend class FbxLoader;

	//�T�u�N���X
	struct VertexPosNormalUvSkin
	{
		Vector3 pos;
		Vector3 normal;
		Vector3 uv;
		UINT boneIndex[MAX_BONE_INDICES];	//�{�[���ԍ�
		float boneWeight[MAX_BONE_INDICES];	//�{�[���d��
	};

public:
	//�f�X�g���N�^
	~FbxObjectManager();

	/// <summary>
	/// �o�b�t�@����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	void CreateBuffers(ID3D12Device* device);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="commandList">�O���t�B�b�N�X�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// ���f���̕ό`�s��擾
	/// </summary>
	/// <returns></returns>
	const XMMATRIX& GetModelTransform() {return meshNode->globalTransform; }

	//getter
	FbxScene* GetFbxScene()	{return fbxScene;}

	std::vector<Bone>& GetBones() {return bones;}

private:
	//FBX�V�[��
	FbxScene* fbxScene = nullptr;

	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<Node> nodes;

	//���b�V�������m�[�h
	Node* meshNode = nullptr;
	//���_�f�[�^�z��
	std::vector<VertexPosNormalUvSkin> vertices;
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

	//�A���r�G���g�W��
	Vector3 ambient = {1, 1, 1};
	//�f�B�q���[�Y�W��
	Vector3 diffuse = {1, 1, 1};
	//�e�N�X�`�����^�f�[�^
	DirectX::TexMetadata metadata = {};
	//�X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg = {};


	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//�{�[���z��
	std::vector<Bone> bones;
};

