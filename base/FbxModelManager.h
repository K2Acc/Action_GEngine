#pragma once

#include "Vector3.h"

#include <DirectXMath.h>
#include <string>
#include <wrl.h>
#include <DirectXTex.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

struct Node{
	//���O
	std::string name;
	DirectX::XMVECTOR scaling = {1,1,1,0};
	DirectX::XMVECTOR rotation = {0,0,0,0};
	DirectX::XMVECTOR translation = {0,0,0,1};
	DirectX::XMMATRIX transform;
	DirectX::XMMATRIX globalTransform;
	Node* parent = nullptr;
};

struct Bone{
	std::string name;
	//�����p��
	DirectX::XMMATRIX invInitialPose;
	//�N���X�^�[
	FbxCluster* fbxCluster;
	//�R���X�g���N�^
	Bone(const std::string& name){
		this->name = name;
	}
};

/// <summary>
/// ���f��
/// </summary>
class FbxModelManager
{
/// <summary>
/// �萔
/// </summary>
public:
	static const int MAX_BONE_INDICES = 4;

/// <summary>
/// �G�C���A�X
/// </summary>
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	template <class T> using vector = std::vector<T>;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	using string = std::string;

/// <summary>
/// �T�u�N���X�E�t�����h�N���X
/// </summary>
public:
	friend class FbxLoader;

	struct VertexPosNormalUvSkin
	{
		Vector3 pos;
		Vector3 normal;
		Vector3 uv;
		UINT boneIndex[MAX_BONE_INDICES];	//�{�[���ԍ�
		float boneWeight[MAX_BONE_INDICES];	//�{�[���d��
	};

public:
	static FbxModelManager* GetInstance();

/// <summary>
/// �����o�֐�
/// </summary>
public:

	/// <summary>
	/// �o�b�t�@����
	/// </summary>
	void CreateBuffers();


	//getter

	//���f���̕ό`�s��
	const XMMATRIX& GetModelTransform() {return meshNode->globalTransform; }
	//FBX�V�[��
	FbxScene* GetFbxScene()	{return fbxScene;}
	//�{�[��
	vector<Bone>& GetBones() {return bones;}
	//�C���f�b�N�X
	uint16_t GetIndices()	{return (UINT)indices.size();}
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW GetvbView()	{return vbView;}
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW GetibView()		{return ibView;}
	//�f�X�N���v�^�q�[�v
	ID3D12DescriptorHeap* GetDescriptorHeap()	{return descriptorHeapSRV.Get();}

/// <summary>
/// �����o�ϐ�
/// </summary>
private:
	//FBX
	FbxScene* fbxScene = nullptr;

	//���f����
	string name;
	//�m�[�h�z��
	vector<Node> nodes;

	//���b�V�������m�[�h
	Node* meshNode = nullptr;

	//���_�f�[�^
	vector<VertexPosNormalUvSkin> vertices;
	//�C���f�b�N�X
	vector<uint16_t> indices;

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> verticesBuffer;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuffer;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> textureBuffer;

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};

	//���_�}�b�v
	VertexPosNormalUvSkin* verticesMap = nullptr;
	//�C���f�b�N�X�}�b�v
	uint16_t* indicesMap = nullptr;

	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descriptorHeapSRV;

	//�{�[���z��
	vector<Bone> bones;

	//�A���r�G���g�W��
	Vector3 ambient = {1,1,1};
	//�f�B�q���[�Y�W��
	Vector3 diffuse = {1,1,1};
	//�e�N�X�`�����^�f�[�^
	TexMetadata metaData{};
	//�X�N���b�`�C���[�W
	ScratchImage scratchImage{};
};

